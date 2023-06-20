; ModuleID = 'call.cpp'
source_filename = "call.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: mustprogress noinline sspstrong uwtable
define dso_local void @_Z1Ei(i32 noundef %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, ptr %a.addr, align 4
  %1 = load i32, ptr %a.addr, align 4
  call void @_Z1Ei(i32 noundef %1)
  ret void
}

; Function Attrs: mustprogress noinline sspstrong uwtable
define dso_local void @_Z1Ci(i32 noundef %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %1 = load i32, ptr %a.addr, align 4
  %mul = mul nsw i32 %0, %1
  store i32 %mul, ptr %a.addr, align 4
  %2 = load i32, ptr %a.addr, align 4
  call void @_Z1Ei(i32 noundef %2)
  ret void
}

; Function Attrs: mustprogress noinline sspstrong uwtable
define dso_local void @_Z1Bv() #0 {
entry:
  %a = alloca i32, align 4
  store i32 100, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  call void @_Z1Ci(i32 noundef %0)
  ret void
}

; Function Attrs: mustprogress noinline nounwind sspstrong uwtable
define dso_local void @_Z1Di(i32 noundef %a) #1 {
entry:
  %a.addr = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %dec = add nsw i32 %0, -1
  store i32 %dec, ptr %a.addr, align 4
  ret void
}

; Function Attrs: mustprogress noinline sspstrong uwtable
define dso_local void @_Z1Av() #0 {
entry:
  %a = alloca i32, align 4
  %0 = load i32, ptr %a, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, ptr %a, align 4
  %1 = load i32, ptr %a, align 4
  call void @_Z1Di(i32 noundef %1)
  %2 = load i32, ptr %a, align 4
  call void @_Z1Ei(i32 noundef %2)
  ret void
}

; Function Attrs: mustprogress noinline norecurse sspstrong uwtable
define dso_local noundef i32 @main() #2 {
entry:
  %retval = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  call void @_Z1Av()
  call void @_Z1Bv()
  ret i32 0
}

attributes #0 = { mustprogress noinline sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline nounwind sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline norecurse sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 15.0.7"}
