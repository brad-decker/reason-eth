[@bs.val] external browserEnv: bool = "process.browser";

[@react.component]
let default = () => {
  let accounts = Hooks.useAccounts();
  <div>
    {switch (accounts) {
     | Resolved(accts) => <AccountBalance account={Array.get(accts, 0)} />
     | Rejected(err) => err->Error.toString->React.string
     | Pending => "Waiting"->React.string
     }}
  </div>;
};