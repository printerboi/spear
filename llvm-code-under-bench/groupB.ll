define i32 @main() #0 {
    %1 = icmp sgt i32 10, 2
    br i1 %1, label %2, label %3

  2:                                                ; preds = %0
    br label %4

  3:                                                ; preds = %0
    br label %4

  4:                                                ; preds = %6, %5
    ret i32 0
}