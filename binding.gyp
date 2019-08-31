{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/asyncScrapBooks.cc" ],
      "include_dirs" : ["<!(node -e \"require('nan')\")"],
      "libraries": [ "<!(pwd)/scrapBooks.so" ]
    }
  ]
}