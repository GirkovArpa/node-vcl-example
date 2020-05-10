const { MyWinMain } = require('bindings')('/release/addon');

MyWinMain(message => console.log(message), 999);

(async () => {
  for (let i = 0; i < Infinity; i++) {
    await new Promise(resolve => setTimeout(resolve, 1000));
    console.log("Tick!");
  }
})();