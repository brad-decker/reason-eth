[@bs.val] external browserEnv: bool = "process.browser";

[@react.component]
let default = () => {
  let eth = Eth.make();
  switch (eth.status) {
  | Connected =>
    Methods.gasPrice()->Promise.tapOk(v => Js.Console.log(v));
    ();
  | Disconnected => Js.Console.log(eth.chain->Chain.toId)
  };
  <div> "hello"->React.string </div>;
};