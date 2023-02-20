; ModuleID = 'loopifcombined.cpp'
source_filename = "loopifcombined.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: mustprogress noinline norecurse nounwind sspstrong uwtable
define dso_local noundef i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %j7 = alloca i32, align 4
  %i18 = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 10, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %cmp = icmp slt i32 %0, 50
  br i1 %cmp, label %if.then, label %if.else17

if.then:                                          ; preds = %entry
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc14, %if.then
  %1 = load i32, ptr %i, align 4
  %cmp1 = icmp slt i32 %1, 1
  br i1 %cmp1, label %for.body, label %for.end16

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %a, align 4
  %3 = load i32, ptr %i, align 4
  %add = add nsw i32 %2, %3
  %cmp2 = icmp slt i32 %add, 40
  br i1 %cmp2, label %if.then3, label %if.else

if.then3:                                         ; preds = %for.body
  store i32 0, ptr %j, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %if.then3
  %4 = load i32, ptr %j, align 4
  %cmp5 = icmp slt i32 %4, 1
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %5 = load i32, ptr %a, align 4
  %div = sdiv i32 %5, 2
  store i32 %div, ptr %a, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body6
  %6 = load i32, ptr %j, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, ptr %j, align 4
  br label %for.cond4, !llvm.loop !6

for.end:                                          ; preds = %for.cond4
  br label %if.end

if.else:                                          ; preds = %for.body
  store i32 0, ptr %j7, align 4
  br label %for.cond8

for.cond8:                                        ; preds = %for.inc11, %if.else
  %7 = load i32, ptr %j7, align 4
  %cmp9 = icmp slt i32 %7, 1
  br i1 %cmp9, label %for.body10, label %for.end13

for.body10:                                       ; preds = %for.cond8
  %8 = load i32, ptr %a, align 4
  %mul = mul nsw i32 %8, 2
  store i32 %mul, ptr %a, align 4
  br label %for.inc11

for.inc11:                                        ; preds = %for.body10
  %9 = load i32, ptr %j7, align 4
  %inc12 = add nsw i32 %9, 1
  store i32 %inc12, ptr %j7, align 4
  br label %for.cond8, !llvm.loop !8

for.end13:                                        ; preds = %for.cond8
  br label %if.end

if.end:                                           ; preds = %for.end13, %for.end
  br label %for.inc14

for.inc14:                                        ; preds = %if.end
  %10 = load i32, ptr %i, align 4
  %inc15 = add nsw i32 %10, 1
  store i32 %inc15, ptr %i, align 4
  br label %for.cond, !llvm.loop !9

for.end16:                                        ; preds = %for.cond
  br label %if.end25

if.else17:                                        ; preds = %entry
  store i32 0, ptr %i18, align 4
  br label %for.cond19

for.cond19:                                       ; preds = %for.inc22, %if.else17
  %11 = load i32, ptr %i18, align 4
  %cmp20 = icmp slt i32 %11, 500
  br i1 %cmp20, label %for.body21, label %for.end24

for.body21:                                       ; preds = %for.cond19
  %12 = load i32, ptr %a, align 4
  %dec = add nsw i32 %12, -1
  store i32 %dec, ptr %a, align 4
  br label %for.inc22

for.inc22:                                        ; preds = %for.body21
  %13 = load i32, ptr %i18, align 4
  %inc23 = add nsw i32 %13, 1
  store i32 %inc23, ptr %i18, align 4
  br label %for.cond19, !llvm.loop !10

for.end24:                                        ; preds = %for.cond19
  br label %if.end25

if.end25:                                         ; preds = %for.end24, %for.end16
  ret i32 0
}

attributes #0 = { mustprogress noinline norecurse nounwind sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 15.0.7"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
