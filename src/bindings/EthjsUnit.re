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