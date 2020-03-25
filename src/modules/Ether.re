open External.EthUnit;
type wei = Bn.t;

type t =
  | Wei(wei)
  | Ether(wei);

let convert = (currency, to_) =>
  switch (currency, to_) {
  | (Wei(wei), `ether) => Ether(wei)
  | (Wei(wei), `wei) => Wei(wei)
  | (Ether(wei), `wei) => Wei(wei)
  | (Ether(wei), `ether) => Ether(wei)
  };

let toString =
  fun
  | Wei(wei) => fromWei(`BigNumber(wei), `wei)
  | Ether(wei) => fromWei(`BigNumber(wei), `ether);

let toStringWithUnit =
  fun
  | Wei(wei) => fromWei(`BigNumber(wei), `wei) ++ " WEI"
  | Ether(wei) => fromWei(`BigNumber(wei), `ether) ++ " ETH";

let fromString = (str, unit) => Wei(toWei(`String(str), unit));