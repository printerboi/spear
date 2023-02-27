define i32 @main() #0 {
  %ptr = alloca i32
  %val = load i32, i32* %ptr
  ret i32 0
}