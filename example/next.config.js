const withTM = require("next-transpile-modules")(["bs-platform", "reason-promise"]);

module.exports = withTM({
  pageExtensions: ["jsx", "js", "bs.js", "tsx"]
});
