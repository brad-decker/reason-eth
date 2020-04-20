type error =
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

type success('a) = {
  result: 'a,
  id: int,
  jsonrpc: string,
};

type errorMessage = {
  code: int,
  message: string,
};

type failure = {
  id: int,
  jsonrpc: string,
  error: errorMessage,
};

type response('a) = Promise.t(result('a, error));
type promise('a) = Promise.Js.t(success('a), failure);

let getRPCResult = r => r.result;

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

let convertResponse = (p: promise('a)): response('a) =>
  Promise.Js.toResult(p)
  ->Promise.mapError(deriveJSONRPCError)
  ->Promise.mapOk(getRPCResult);

let call = (thunk: unit => promise('a)): response('a) => {
  switch (Eth.ethereum(Webapi.Dom.window)) {
  | exception _ => Promise.resolved(Error(NotConnectedToProvider))
  | None => Promise.resolved(Error(NotConnectedToProvider))
  | Some(_) => thunk()->convertResponse
  };
};