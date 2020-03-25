type promiseState('a, 'e) =
  | Resolved('a)
  | Rejected('e)
  | Pending;
let usePromise = (promise: Promise.t(result('a, 'e))) => {
  let (state, setState) = React.useState(_ => Pending);
  React.useEffect1(
    () => {
      let subscribed = ref(true);
      promise->Promise.get(res =>
        if (subscribed^ == true) {
          switch (res) {
          | Ok(v) => setState(_ => Resolved(v))
          | Error(e) => setState(_ => Rejected(e))
          };
        }
      );

      Some(() => {subscribed := false});
    },
    [|promise|],
  );
  state;
};

let useEthPromise = (fn: unit => Promise.t(result('a, 'e))) => {
  let p =
    React.useMemo1(
      () => {
        switch (Eth.getStatus()) {
        | Connected => fn()
        | Disconnected =>
          Promise.resolved(Error(Eth_Error.NotConnectedToProvider))
        }
      },
      [|fn|],
    );
  usePromise(p);
};

let useAccounts = () => {
  let fn = React.useCallback0(() => Methods.requestAccounts());
  useEthPromise(fn);
};

let useBalance = (account: string, blockIdentifier: Format.BlockIdentifier.t) => {
  let fn =
    React.useCallback2(
      () => Methods.getBalance(account, blockIdentifier),
      (account, blockIdentifier),
    );
  useEthPromise(fn);
};