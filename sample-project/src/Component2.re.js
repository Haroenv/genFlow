/* @flow strict */

var Component2 = require("./Component2.bs");
var CreateBucklescriptBlock = require("bs-platform/lib/js/block.js");

// No need to import locally visible type block. Make sure it is also marked with @genFlow;
const getBlock = Component2.getBlock;
export opaque type BlockBlock = any // Reason type already checked. Making it opaque;
const Block = 0;
export type block =
  | BlockBlock;
export opaque type VariantA = any // Reason type already checked. Making it opaque;
const A = 0;
export opaque type VariantB = any // Reason type already checked. Making it opaque;
function B(Arg1, Arg2) { CreateBucklescriptBlock.__(0, [Arg1, Arg2]) }
export opaque type VariantC = any // Reason type already checked. Making it opaque;
function C(Arg1) { CreateBucklescriptBlock.__(1, [(Arg1 === null ? undefined : Arg1)]) }
export type variant =
  | VariantA
  | VariantB
  | VariantC;

exports.getBlock = (getBlock: (block) => number);
exports.Block = (Block: BlockBlock);
exports.A = (A: VariantA);
exports.B = (B: (number, number) => VariantB);
exports.C = (C: (?number) => VariantC);