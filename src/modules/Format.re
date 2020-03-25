[@bs.module]
external numberToBN:
  (
  [@bs.unwrap]
  [ | `Int(int) | `Float(float) | `String(string) | `BigNumber(Bn.t)]
  ) =>
  Bn.t =
  "number-to-bn";

let utf8padding = [%re "/^(?:\u0000)*/"];
let hexpadding = [%re "/^(?:00)*/"];
let isHexStrictRegex = [%re "/^(-)?0x[0-9a-f]*$/i"];
let isHexRegex = [%re "/^(-0x|0x)?[0-9a-f]*$/i"];

let isHex = s => Js.Re.test_(isHexRegex, s);
let isHexStrict = s => Js.Re.test_(isHexStrictRegex, s);

let ten = Bn.fromFloat(10.);
let eighteen = Bn.fromFloat(18.);
let oneEth = Bn.pow(eighteen, ten);
Js.log2(ten, eighteen);

/**
 * Returns a new string that is the reverse of the provided
 * string.
 */
let reverseString = s =>
  s->Js.String2.split("")->Belt.Array.reverse->Js.Array2.joinWith("");

/**
 * Strip padding from both the beginning and end of a string
 */
let removePadding = (s, regex) =>
  s
  ->Js.String2.replaceByRe(regex, "")
  ->reverseString
  ->Js.String2.replaceByRe(regex, "")
  ->reverseString;

module Data = {
  type t = [ | `PlainText(string) | `HexString(string)];
  let plainText = str => `PlainText(str);
  let hexString = hex => `HexString(hex);

  let extractCleanString = data => {
    switch (data) {
    | `PlainText(str) =>
      External.Utf8.encode(str)->removePadding(utf8padding)
    | `HexString(str) =>
      str->Js.String2.replace("0x", "")->removePadding(hexpadding)
    };
  };

  let prepareData = data =>
    switch (data) {
    | `PlainText(str) =>
      External.Utf8.encode(str)->removePadding(utf8padding)
    | `HexString(str) =>
      str->Js.String2.replace("0x", "")->removePadding(hexpadding)
    };

  let hexCharToStr = hex => {
    let it = External.parseInt(hex, 16);
    let code = it->Js.String2.fromCharCode;
    code;
  };

  let rec reverseHex = (~curr="", hex) => {
    let currentChar = hex->Js.String2.substrAtMost(~from=0, ~length=2);
    let appended = curr ++ currentChar->hexCharToStr;
    if (hex->Js.String2.length > 2) {
      reverseHex(~curr=appended, hex->Js.String2.sliceToEnd(~from=2));
    } else {
      appended;
    };
  };

  /**
 * Convert a string into a hexadecimal string for sending
 * in transactions.
 */
  let encode = (input: [ | `PlainText(string)]) => {
    hexString(
      input
      ->prepareData
      ->Js.String2.split("")
      ->Belt.Array.reduce("0x", (result, character) =>
          result
          ++ (
            switch (Js.String.codePointAt(0, character)) {
            | Some(v) =>
              let vstr = v->Js.Int.toStringWithRadix(~radix=16);
              vstr->Js.String.length < 2 ? "0" ++ vstr : vstr;
            | None => ""
            }
          )
        ),
    );
  };

  let decode = (input: [ | `HexString(string)]) => {
    plainText(input->prepareData->reverseHex->External.Utf8.decode);
  };

  let getString = t => {
    switch (t) {
    | `PlainText(v) => v
    | `HexString(v) => v
    };
  };
};

module Quantity = {
  [@unboxed]
  type t =
    | Any('a): t;

  type case =
    | HexString(string)
    | BigNumber(Bn.t);

  let string = (x: string) => Any(x);
  let int = (x: int) => Any(x);
  let classify = (Any(v): t): case =>
    if (Js.typeof(v) == "string") {
      HexString(Obj.magic(v): string);
    } else {
      BigNumber(numberToBN(`String(Obj.magic(v): string)));
    };

  let caseToBn =
    fun
    | HexString(s) =>
      if (!isHexStrict(s)) {
        Js.Exn.raiseError("no");
      } else {
        numberToBN(`String(s));
      }
    | BigNumber(b) => b;

  let toBn = (v: t) => v->classify->caseToBn;

  let toFloat = (x: case) => {
    let bn = caseToBn(x);
    Bn.toNumber(bn);
  };

  let toHex = (x: case) => {
    let bn = caseToBn(x);
    let s = Bn.toString(~base=16, bn);
    if (Bn.lt(bn, Bn.fromFloat(0.))) {
      "-0x" ++ s->Js.String.substr(~from=1);
    } else {
      "0x" ++ s;
    };
  };
  let toNumberString = (x: case) => {
    let bn = caseToBn(x);
    Bn.toString(~base=10, bn);
  };
};

module BlockIdentifier = {
  type t =
    | Latest
    | Earliest
    | Pending
    | Block(Data.t);

  let toString =
    fun
    | Latest => "latest"
    | Earliest => "earliest"
    | Pending => "pending"
    | Block(`HexString(st)) => st
    | Block(`PlainText(st)) => Data.encode(`PlainText(st))->Data.getString;
};

Js.log(Bn.toString(~base=10, oneEth));