type t =
  | Mainnet
  | Morden
  | Ropsten
  | Rinkeby
  | Goerli
  | Kovan
  | NotConnected
  | Other(string);

let fromId = chainId => {
  switch (chainId) {
  | "0x1" => Mainnet
  | "0x2" => Morden
  | "0x3" => Ropsten
  | "0x4" => Rinkeby
  | "0x5" => Goerli
  | "0x42" => Kovan
  | "0x0000" => NotConnected
  | x => Other(x)
  };
};

let toName = chain => {
  switch (chain) {
  | Mainnet => "Mainnet"
  | Morden => "Morden"
  | Ropsten => "Ropsten"
  | Rinkeby => "Rinkeby"
  | Goerli => "Goerli"
  | Kovan => "Kovan"
  | NotConnected => "Not Connected"
  | Other(x) => x
  };
};

let toId = chain => {
  switch (chain) {
  | Mainnet => "0x1"
  | Morden => "0x2"
  | Ropsten => "0x3"
  | Rinkeby => "0x4"
  | Goerli => "0x5"
  | Kovan => "0x42"
  | NotConnected => "0x0000"
  | Other(x) => x
  };
};