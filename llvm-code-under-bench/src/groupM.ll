define i32 @main() #0 {
  %1 = alloca i32*, align 8
  %2 = alloca i32, align 4
  store i32 42, i32* %2, align 4
  store i32* %2, i32** %1, align 8
  ret i32 0
}