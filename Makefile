default: build

compile-go:
	go build -buildmode=c-shared -o scrapBooks.so src/golang/scrapBooks.go

build: compile-go
	yarn run build