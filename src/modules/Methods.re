type quantity = string;

/**
 * Request a user's ethereum accounts
 */
[@bs.scope "ethereum"] [@bs.val]
external requestAccounts:
  ([@bs.string] [ | `eth_requestAccounts]) => JsonRPC.promise(array(string)) =
  "send";

let requestAccounts = () =>
  JsonRPC.call(() => requestAccounts(`eth_requestAccounts));

/**
 * Get the current gas price. Converts the hexadecimal string
 * into an integer for the user.
 */
[@bs.scope "ethereum"] [@bs.val]
external gasPrice:
  ([@bs.string] [ | `eth_gasPrice]) => JsonRPC.promise(string) =
  "send";

let gasPrice = () =>
  JsonRPC.call(() => gasPrice(`eth_gasPrice))
  ->Promise.mapOk(result => result->int_of_string);

[@bs.scope "ethereum"] [@bs.val]
external sign:
  ([@bs.string] [ | `eth_sign], (string, string)) => JsonRPC.promise(string) =
  "send";

let sign = (account: string, data: Format.Data.t) => {
  let encodedData =
    switch (data) {
    | `PlainText(d) =>
      Format.Data.encode(`PlainText(d))->Format.Data.extractCleanString
    | `HexString(d) => d
    };
  JsonRPC.call(() => {sign(`eth_sign, (account, encodedData))});
};

[@bs.scope "ethereum"] [@bs.val]
external getBalance:
  ([@bs.string] [ | `eth_getBalance], (string, string)) =>
  JsonRPC.promise(quantity) =
  "send";

let getBalance = (account, blockIdentifier: Format.BlockIdentifier.t) =>
  JsonRPC.call(() =>
    getBalance(
      `eth_getBalance,
      (account, blockIdentifier->Format.BlockIdentifier.toString),
    )
  )
  ->Promise.mapOk(result => result->Ether.fromString(`wei));