[@bs.val] external window: _ = "window";
let isBrowser = Js.typeof(window) != "undefined";

type t = {
  isMetaMask: bool,
  chainId: string,
  enable: unit => Promise.Js.t(array(string), Js.Exn.t),
};

[@bs.val] external ethereumJs: Js.Nullable.t(t) = "ethereum";
[@bs.scope "ethereum"] [@bs.val]
external requestAccounts: unit => Promise.Js.t(array(string), Js.Exn.t) =
  "send";

let make = () =>
  if (!isBrowser) {
    None;
  } else {
    ethereumJs->Js.Nullable.toOption;
  };