type jsonRPCResult('a) = {
  result: 'a,
  id: int,
  jsonrpc: string,
};

[@bs.scope "ethereum"] [@bs.val]
external requestAccountsJs:
  ([@bs.string] [ | `eth_requestAccounts]) =>
  Promise.Js.t(jsonRPCResult(array(string)), Js.Exn.t) =
  "send";

[@bs.scope "ethereum"] [@bs.val]
external gasPriceJs:
  ([@bs.string] [ | `eth_gasPrice]) =>
  Promise.Js.t(jsonRPCResult(string), Js.Exn.t) =
  "send";

/**
 * Request a user's ethereum accounts
 */
let requestAccounts = () =>
  requestAccountsJs(`eth_requestAccounts)
  ->Promise.Js.toResult
  ->Promise.mapOk(res => res.result);

/**
 * Get the current gas price. Converts the hexadecimal string
 * into an integer for the user.
 */
let gasPrice = () =>
  gasPriceJs(`eth_gasPrice)
  ->Promise.Js.toResult
  ->Promise.mapOk(res => res.result->int_of_string);