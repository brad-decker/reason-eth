module Utf8 = {
  [@bs.module "utf8"] external encode: string => string = "encode";
  [@bs.module "utf8"] external decode: string => string = "decode";
};

module EthUnit = {
  [@bs.module "ethjs-unit"]
  external toWei:
    (
      [@bs.unwrap] [ | `String(string) | `BigNumber(Bn.t)],
      [@bs.string] [ | `wei | `ether]
    ) =>
    Bn.t =
    "toWei";

  [@bs.module "ethjs-unit"]
  external fromWei:
    (
      [@bs.unwrap] [ | `String(string) | `BigNumber(Bn.t)],
      [@bs.string] [ | `wei | `ether]
    ) =>
    string =
    "fromWei";
};

// Integer parsing with redix
[@bs.val] external parseInt: (string, int) => int = "parseInt";