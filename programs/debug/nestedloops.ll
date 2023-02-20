; ModuleID = 'nestedloops.cpp'
source_filename = "nestedloops.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: mustprogress noinline norecurse nounwind sspstrong uwtable
define dso_local noundef i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %k8 = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 0, ptr %a, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc18, %entry
  %0 = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end20

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc15, %for.body
  %1 = load i32, ptr %j, align 4
  %cmp2 = icmp slt i32 %1, 1
  br i1 %cmp2, label %for.body3, label %for.end17

for.body3:                                        ; preds = %for.cond1
  %2 = load i32, ptr %a, align 4
  %cmp4 = icmp eq i32 %2, 0
  br i1 %cmp4, label %if.then, label %if.else

if.then:                                          ; preds = %for.body3
  store i32 0, ptr %k, align 4
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc, %if.then
  %3 = load i32, ptr %k, align 4
  %cmp6 = icmp slt i32 %3, 1
  br i1 %cmp6, label %for.body7, label %for.end

for.body7:                                        ; preds = %for.cond5
  br label %for.inc

for.inc:                                          ; preds = %for.body7
  %4 = load i32, ptr %k, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %k, align 4
  br label %for.cond5, !llvm.loop !6

for.end:                                          ; preds = %for.cond5
  br label %if.end

if.else:                                          ; preds = %for.body3
  store i32 0, ptr %k8, align 4
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc12, %if.else
  %5 = load i32, ptr %k8, align 4
  %cmp10 = icmp slt i32 %5, 1
  br i1 %cmp10, label %for.body11, label %for.end14

for.body11:                                       ; preds = %for.cond9
  br label %for.inc12

for.inc12:                                        ; preds = %for.body11
  %6 = load i32, ptr %k8, align 4
  %inc13 = add nsw i32 %6, 1
  store i32 %inc13, ptr %k8, align 4
  br label %for.cond9, !llvm.loop !8

for.end14:                                        ; preds = %for.cond9
  br label %if.end

if.end:                                           ; preds = %for.end14, %for.end
  br label %for.inc15

for.inc15:                                        ; preds = %if.end
  %7 = load i32, ptr %j, align 4
  %inc16 = add nsw i32 %7, 1
  store i32 %inc16, ptr %j, align 4
  br label %for.cond1, !llvm.loop !9

for.end17:                                        ; preds = %for.cond1
  br label %for.inc18

for.inc18:                                        ; preds = %for.end17
  %8 = load i32, ptr %i, align 4
  %inc19 = add nsw i32 %8, 1
  store i32 %inc19, ptr %i, align 4
  br label %for.cond, !llvm.loop !10

for.end20:                                        ; preds = %for.cond
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
