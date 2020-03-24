// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var EthjsUnit = require("ethjs-unit");

function convert(currency, to_) {
  var wei = currency[0];
  if (to_ >= 5940379) {
    return /* Wei */Block.__(0, [wei]);
  } else {
    return /* Ether */Block.__(1, [wei]);
  }
}

function toString(param) {
  if (param.tag) {
    return EthjsUnit.fromWei(param[0], "ether");
  } else {
    return EthjsUnit.fromWei(param[0], "wei");
  }
}

function toStringWithUnit(param) {
  if (param.tag) {
    return EthjsUnit.fromWei(param[0], "ether") + " ETH";
  } else {
    return EthjsUnit.fromWei(param[0], "wei") + " WEI";
  }
}

function fromString(str, unit) {
  return /* Wei */Block.__(0, [EthjsUnit.toWei(str, (function () {
                      switch (unit) {
                        case 5940379 :
                            return "wei";
                        case -193685050 :
                            return "ether";
                        
                      }
                    })())]);
}

exports.convert = convert;
exports.toString = toString;
exports.toStringWithUnit = toStringWithUnit;
exports.fromString = fromString;
/* ethjs-unit Not a pure module */
