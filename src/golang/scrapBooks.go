package main

import "C"
import "strings"

//export GetBookTitle
func GetBookTitle(input *C.char) *C.char {
	var str strings.Builder

	str.WriteString(C.GoString(input))
	str.WriteString(" : Nodejs, Go & C++!!")

	return C.CString(str.String())
}

func main() {
}
