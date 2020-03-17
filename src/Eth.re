type status =
  | Disconnected
  | Connected;

type t = {
  status,
  chain: Chain.t,
  enable: unit => Promise.t(result(array(string), Js.Exn.t)),
  isMetaMask: bool,
};

let make = () => {
  switch (EthJS.make()) {
  | Some(eth) => {
      chain: eth.chainId->Chain.fromId,
      enable: () => {
        eth.enable()->Promise.Js.toResult;
      },
      isMetaMask: eth.isMetaMask,
      status: Connected,
    }

  | None => {
      enable: () => Promise.resolved(Ok([||])),
      status: Disconnected,
      isMetaMask: false,
      chain: NotConnected,
    }
  };
};