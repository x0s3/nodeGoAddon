try {
  module.exports = require('../build/Release/addon.node');
} catch (error) {
  module.exports = require('../build/Debug/addon.node');
}
