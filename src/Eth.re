type status =
  | Disconnected
  | Connected;

type t = {
  status,
  chain: Chain.t,
  isMetaMask: bool,
};

type ethjs = {
  isMetaMask: bool,
  chainId: string,
  enable: unit => Promise.Js.t(array(string), Js.Exn.t),
};

[@bs.val] external window: _ = "window";
let isBrowser = Js.typeof(window) != "undefined";

[@bs.get] [@bs.return nullable]
external ethereum: Dom.window => option(ethjs) = "ethereum";

let defaultState = {
  status: Disconnected,
  isMetaMask: false,
  chain: NotConnected,
};

let assertEnv = () =>
  if (!isBrowser || Belt.Option.isNone(ethereum(Webapi.Dom.window))) {
    Error.raiseJsExn(NotConnectedToProvider);
  } else {
    ();
  };

let make = () =>
  if (!isBrowser) {
    defaultState;
  } else {
    switch (ethereum(Webapi.Dom.window)) {
    | Some(eth) => {
        chain: eth.chainId->Chain.fromId,
        isMetaMask: eth.isMetaMask,
        status: Connected,
      }
    | None => defaultState
    };
  };