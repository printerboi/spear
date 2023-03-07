define void @_Z3foov() #0 {
  ret void
}

define i32 @main() #1 {
  call void @_Z3foov()
  ret i32 0
}