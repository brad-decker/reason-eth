type status =
  | Disconnected
  | Connected;

type t = {
  isMetaMask: bool,
  chainId: string,
  enable: unit => Promise.Js.t(array(string), Js.Exn.t),
};

[@bs.get] [@bs.return nullable]
external ethereum: Dom.window => option(t) = "ethereum";

let call =
    (thunk: unit => Promise.t(result('a, Eth_Error.t)))
    : Promise.t(result('a, Eth_Error.t)) => {
  switch (ethereum(Webapi.Dom.window)) {
  | exception _ => Promise.resolved(Error(Eth_Error.NotConnectedToProvider))
  | None => Promise.resolved(Error(Eth_Error.NotConnectedToProvider))
  | Some(_) => thunk()
  };
};

let getChain = (): Chain.t => {
  switch (ethereum(Webapi.Dom.window)) {
  | exception _ => NotConnected
  | None => NotConnected
  | Some(eth) => eth.chainId->Chain.fromId
  };
};

let isMetaMask = () => {
  switch (ethereum(Webapi.Dom.window)) {
  | exception _ => false
  | None => false
  | Some(eth) => eth.isMetaMask
  };
};

let getStatus = () => {
  switch (ethereum(Webapi.Dom.window)) {
  | exception _ => Disconnected
  | None => Disconnected
  | Some(_) => Connected
  };
};