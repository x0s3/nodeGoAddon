'use strict';

const nativeGo = require('./src');

console.log(nativeGo);

nativeGo.getAsyncBookTitle('/BOOK_ID', data => console.log(data));

console.log('after native callback :)');
