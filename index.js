'use strict';

const nativeGo = require('./src');

console.log('JS', nativeGo);

// nativeGo.getSyncBookTitle('/BOOK_ID', data =>
//   console.log(`JS callback data: ${data}`)
// );

nativeGo.getAsyncBookTitle('/BOOKD_ID', data =>
  console.log(`JS callback data: ${data}`)
);

console.log('JS after native callback :)');
