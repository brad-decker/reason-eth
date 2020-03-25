type rpcOk('a) = {
  result: 'a,
  id: int,
  jsonrpc: string,
};

type jsonRPCResult('a) = result(rpcOk('a), Error.jsonRPCError);
type jsonRPCPromise('a) = Promise.Js.t(rpcOk('a), Error.jsonRPCError);
type quantity = string;

let getRPCResult = r => r.result;

[@bs.scope "ethereum"] [@bs.val]
external signJs:
  ([@bs.string] [ | `eth_sign], (string, string)) => jsonRPCPromise(string) =
  "send";

/**
 * Request a user's ethereum accounts
 */
[@bs.scope "ethereum"] [@bs.val]
external requestAccounts:
  ([@bs.string] [ | `eth_requestAccounts]) => jsonRPCPromise(array(string)) =
  "send";

let requestAccounts = () => {
  Eth.assertEnv();
  requestAccounts(`eth_requestAccounts)
  ->Promise.Js.toResult
  ->Promise.mapOk(getRPCResult)
  ->Promise.mapError(Error.deriveJSONRPCError);
};

/**
 * Get the current gas price. Converts the hexadecimal string
 * into an integer for the user.
 */
[@bs.scope "ethereum"] [@bs.val]
external gasPrice: ([@bs.string] [ | `eth_gasPrice]) => jsonRPCPromise(string) =
  "send";

let gasPrice = () => {
  Eth.assertEnv();
  gasPrice(`eth_gasPrice)
  ->Promise.Js.toResult
  ->Promise.mapOk(res => res.result->int_of_string)
  ->Promise.mapError(Error.deriveJSONRPCError);
};

let sign = (account: string, data: Format.Data.t) => {
  Eth.assertEnv();
  let encodedData =
    switch (data) {
    | `PlainText(d) =>
      Format.Data.encode(`PlainText(d))->Format.Data.extractCleanString
    | `HexString(d) => d
    };
  signJs(`eth_sign, (account, encodedData))
  ->Promise.Js.toResult
  ->Promise.mapOk(getRPCResult)
  ->Promise.mapError(Error.deriveJSONRPCError);
};

[@bs.scope "ethereum"] [@bs.val]
external getBalance:
  ([@bs.string] [ | `eth_getBalance], (string, string)) =>
  jsonRPCPromise(quantity) =
  "send";

let getBalance = (account, blockIdentifier: Format.BlockIdentifier.t) => {
  Eth.assertEnv();
  getBalance(
    `eth_getBalance,
    (account, blockIdentifier->Format.BlockIdentifier.toString),
  )
  ->Promise.Js.toResult
  ->Promise.mapOk(res => {res.result->Ether.fromString(`wei)})
  ->Promise.mapError(Error.deriveJSONRPCError);
};