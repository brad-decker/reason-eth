type t =
  | ParseError
  | InvalidRequest
  | MethodNotFound
  | InvalidParams
  | InternalError
  | InvalidInput
  | ResourceNotFound
  | ResourceUnavailable
  | TransactionRejected
  | MethodNotSupported
  | LimitExceeded
  | VersionNotSupported
  | UnidentifiedError(option(int), option(string))
  | NotConnectedToProvider;

type error = {
  code: int,
  message: string,
};

type jsonRPCError = {
  id: int,
  jsonrpc: string,
  error,
};

let toString =
  fun
  | ParseError => "Invalid JSON"
  | InvalidRequest => "JSON is not a valid request object"
  | MethodNotFound => "Method does not exist"
  | InvalidParams => "Invalid method parameters"
  | InternalError => "Internal JSON-RPC error"
  | InvalidInput => "Missing or invalid parameters"
  | ResourceNotFound => "Requested resource was not found"
  | ResourceUnavailable => "Requested resource not available"
  | TransactionRejected => "Transaction creation failed"
  | MethodNotSupported => "Method is not implemented"
  | LimitExceeded => "Request exceeds defined limit"
  | VersionNotSupported => "Version of JSON-RPC protocol is not supported"
  | UnidentifiedError(Some(code), Some(message)) => {j|An error occurred with code $code, stating: $message|j}
  | UnidentifiedError(Some(code), None) => {j|An error with code $code occurred|j}
  | UnidentifiedError(None, Some(message)) => message
  | UnidentifiedError(None, None) => "An unknown error occurred"
  | NotConnectedToProvider => "Currently not connected to ethereum provider";

let raiseJsExn = err => Js.Exn.raiseError(err->toString);

let deriveJSONRPCError = jsonError =>
  switch (jsonError.error) {
  | {code: (-32700)} => ParseError
  | {code: (-32600)} => InvalidRequest
  | {code: (-32601)} => MethodNotFound
  | {code: (-32602)} => InvalidParams
  | {code: (-32603)} => InternalError
  | {code: (-32000)} => InvalidInput
  | {code: (-32001)} => ResourceNotFound
  | {code: (-32002)} => ResourceUnavailable
  | {code: (-32003)} => TransactionRejected
  | {code: (-32004)} => MethodNotSupported
  | {code: (-32005)} => LimitExceeded
  | {code: (-32006)} => VersionNotSupported
  | e => UnidentifiedError(Some(e.code), Some(e.message))
  };