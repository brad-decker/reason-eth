[@react.component]
let make = (~account) => {
  let result = Hooks.useBalance(account, Latest);
  <div>
    {switch (result) {
     | Resolved(bal) =>
       <p>
         {bal->Ether.convert(`ether)->Ether.toStringWithUnit->React.string}
       </p>
     | Rejected(e) => <p> {e->Error.toString->React.string} </p>
     | Pending => <p> "Requesting Balance"->React.string </p>
     }}
  </div>;
};