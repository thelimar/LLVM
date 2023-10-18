; ModuleID = '/home/karina/Prog/LLVM_course/maks/life.cpp'
source_filename = "/home/karina/Prog/LLVM_course/maks/life.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@current_gen = dso_local global [240000 x i32] zeroinitializer, align 16
@next_gen = dso_local global [240000 x i32] zeroinitializer, align 16

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local void @_Z7DrawGenv() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  br label %3

3:                                                ; preds = %27, %0
  %4 = load i32, i32* %1, align 4
  %5 = icmp slt i32 %4, 400
  br i1 %5, label %6, label %30

6:                                                ; preds = %3
  store i32 0, i32* %2, align 4
  br label %7

7:                                                ; preds = %23, %6
  %8 = load i32, i32* %2, align 4
  %9 = icmp slt i32 %8, 600
  br i1 %9, label %10, label %26

10:                                               ; preds = %7
  %11 = load i32, i32* %1, align 4
  %12 = mul nsw i32 %11, 600
  %13 = load i32, i32* %2, align 4
  %14 = add nsw i32 %12, %13
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds [240000 x i32], [240000 x i32]* @next_gen, i64 0, i64 %15
  %17 = load i32, i32* %16, align 4
  %18 = icmp eq i32 %17, 1
  br i1 %18, label %19, label %22

19:                                               ; preds = %10
  %20 = load i32, i32* %2, align 4
  %21 = load i32, i32* %1, align 4
  call void @_Z11SimPutPixeliii(i32 noundef %20, i32 noundef %21, i32 noundef -65536)
  br label %22

22:                                               ; preds = %19, %10
  br label %23

23:                                               ; preds = %22
  %24 = load i32, i32* %2, align 4
  %25 = add nsw i32 %24, 1
  store i32 %25, i32* %2, align 4
  br label %7, !llvm.loop !6

26:                                               ; preds = %7
  br label %27

27:                                               ; preds = %26
  %28 = load i32, i32* %1, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, i32* %1, align 4
  br label %3, !llvm.loop !8

30:                                               ; preds = %3
  call void @_Z8SimFlushv()
  ret void
}

declare void @_Z11SimPutPixeliii(i32 noundef, i32 noundef, i32 noundef) #1

declare void @_Z8SimFlushv() #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local void @_Z11CalcNextGenv() #2 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  br label %8

8:                                                ; preds = %119, %0
  %9 = load i32, i32* %1, align 4
  %10 = icmp slt i32 %9, 400
  br i1 %10, label %11, label %122

11:                                               ; preds = %8
  store i32 0, i32* %2, align 4
  br label %12

12:                                               ; preds = %115, %11
  %13 = load i32, i32* %2, align 4
  %14 = icmp slt i32 %13, 600
  br i1 %14, label %15, label %118

15:                                               ; preds = %12
  store i32 0, i32* %3, align 4
  store i32 -1, i32* %4, align 4
  br label %16

16:                                               ; preds = %63, %15
  %17 = load i32, i32* %4, align 4
  %18 = icmp sle i32 %17, 1
  br i1 %18, label %19, label %66

19:                                               ; preds = %16
  store i32 -1, i32* %5, align 4
  br label %20

20:                                               ; preds = %59, %19
  %21 = load i32, i32* %5, align 4
  %22 = icmp sle i32 %21, 1
  br i1 %22, label %23, label %62

23:                                               ; preds = %20
  %24 = load i32, i32* %4, align 4
  %25 = icmp eq i32 %24, 0
  br i1 %25, label %26, label %30

26:                                               ; preds = %23
  %27 = load i32, i32* %5, align 4
  %28 = icmp eq i32 %27, 0
  br i1 %28, label %29, label %30

29:                                               ; preds = %26
  br label %59

30:                                               ; preds = %26, %23
  %31 = load i32, i32* %2, align 4
  %32 = load i32, i32* %4, align 4
  %33 = add nsw i32 %31, %32
  store i32 %33, i32* %6, align 4
  %34 = load i32, i32* %1, align 4
  %35 = load i32, i32* %5, align 4
  %36 = add nsw i32 %34, %35
  store i32 %36, i32* %7, align 4
  %37 = load i32, i32* %6, align 4
  %38 = icmp sge i32 %37, 0
  br i1 %38, label %39, label %58

39:                                               ; preds = %30
  %40 = load i32, i32* %6, align 4
  %41 = icmp slt i32 %40, 600
  br i1 %41, label %42, label %58

42:                                               ; preds = %39
  %43 = load i32, i32* %7, align 4
  %44 = icmp sge i32 %43, 0
  br i1 %44, label %45, label %58

45:                                               ; preds = %42
  %46 = load i32, i32* %7, align 4
  %47 = icmp slt i32 %46, 400
  br i1 %47, label %48, label %58

48:                                               ; preds = %45
  %49 = load i32, i32* %7, align 4
  %50 = mul nsw i32 %49, 600
  %51 = load i32, i32* %6, align 4
  %52 = add nsw i32 %50, %51
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds [240000 x i32], [240000 x i32]* @current_gen, i64 0, i64 %53
  %55 = load i32, i32* %54, align 4
  %56 = load i32, i32* %3, align 4
  %57 = add nsw i32 %56, %55
  store i32 %57, i32* %3, align 4
  br label %58

58:                                               ; preds = %48, %45, %42, %39, %30
  br label %59

59:                                               ; preds = %58, %29
  %60 = load i32, i32* %5, align 4
  %61 = add nsw i32 %60, 1
  store i32 %61, i32* %5, align 4
  br label %20, !llvm.loop !9

62:                                               ; preds = %20
  br label %63

63:                                               ; preds = %62
  %64 = load i32, i32* %4, align 4
  %65 = add nsw i32 %64, 1
  store i32 %65, i32* %4, align 4
  br label %16, !llvm.loop !10

66:                                               ; preds = %16
  %67 = load i32, i32* %1, align 4
  %68 = mul nsw i32 %67, 600
  %69 = load i32, i32* %2, align 4
  %70 = add nsw i32 %68, %69
  %71 = sext i32 %70 to i64
  %72 = getelementptr inbounds [240000 x i32], [240000 x i32]* @current_gen, i64 0, i64 %71
  %73 = load i32, i32* %72, align 4
  %74 = icmp eq i32 %73, 1
  br i1 %74, label %75, label %96

75:                                               ; preds = %66
  %76 = load i32, i32* %3, align 4
  %77 = icmp slt i32 %76, 2
  br i1 %77, label %81, label %78

78:                                               ; preds = %75
  %79 = load i32, i32* %3, align 4
  %80 = icmp sgt i32 %79, 3
  br i1 %80, label %81, label %88

81:                                               ; preds = %78, %75
  %82 = load i32, i32* %1, align 4
  %83 = mul nsw i32 %82, 600
  %84 = load i32, i32* %2, align 4
  %85 = add nsw i32 %83, %84
  %86 = sext i32 %85 to i64
  %87 = getelementptr inbounds [240000 x i32], [240000 x i32]* @next_gen, i64 0, i64 %86
  store i32 0, i32* %87, align 4
  br label %95

88:                                               ; preds = %78
  %89 = load i32, i32* %1, align 4
  %90 = mul nsw i32 %89, 600
  %91 = load i32, i32* %2, align 4
  %92 = add nsw i32 %90, %91
  %93 = sext i32 %92 to i64
  %94 = getelementptr inbounds [240000 x i32], [240000 x i32]* @next_gen, i64 0, i64 %93
  store i32 1, i32* %94, align 4
  br label %95

95:                                               ; preds = %88, %81
  br label %114

96:                                               ; preds = %66
  %97 = load i32, i32* %3, align 4
  %98 = icmp eq i32 %97, 3
  br i1 %98, label %99, label %106

99:                                               ; preds = %96
  %100 = load i32, i32* %1, align 4
  %101 = mul nsw i32 %100, 600
  %102 = load i32, i32* %2, align 4
  %103 = add nsw i32 %101, %102
  %104 = sext i32 %103 to i64
  %105 = getelementptr inbounds [240000 x i32], [240000 x i32]* @next_gen, i64 0, i64 %104
  store i32 1, i32* %105, align 4
  br label %113

106:                                              ; preds = %96
  %107 = load i32, i32* %1, align 4
  %108 = mul nsw i32 %107, 600
  %109 = load i32, i32* %2, align 4
  %110 = add nsw i32 %108, %109
  %111 = sext i32 %110 to i64
  %112 = getelementptr inbounds [240000 x i32], [240000 x i32]* @next_gen, i64 0, i64 %111
  store i32 0, i32* %112, align 4
  br label %113

113:                                              ; preds = %106, %99
  br label %114

114:                                              ; preds = %113, %95
  br label %115

115:                                              ; preds = %114
  %116 = load i32, i32* %2, align 4
  %117 = add nsw i32 %116, 1
  store i32 %117, i32* %2, align 4
  br label %12, !llvm.loop !11

118:                                              ; preds = %12
  br label %119

119:                                              ; preds = %118
  %120 = load i32, i32* %1, align 4
  %121 = add nsw i32 %120, 1
  store i32 %121, i32* %1, align 4
  br label %8, !llvm.loop !12

122:                                              ; preds = %8
  ret void
}

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local void @_Z3appv() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  br label %6

6:                                                ; preds = %26, %0
  %7 = load i32, i32* %1, align 4
  %8 = icmp slt i32 %7, 400
  br i1 %8, label %9, label %29

9:                                                ; preds = %6
  store i32 0, i32* %2, align 4
  br label %10

10:                                               ; preds = %22, %9
  %11 = load i32, i32* %2, align 4
  %12 = icmp slt i32 %11, 600
  br i1 %12, label %13, label %25

13:                                               ; preds = %10
  %14 = call noundef i32 @_Z7SimRandv()
  %15 = srem i32 %14, 2
  %16 = load i32, i32* %1, align 4
  %17 = mul nsw i32 %16, 600
  %18 = load i32, i32* %2, align 4
  %19 = add nsw i32 %17, %18
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [240000 x i32], [240000 x i32]* @current_gen, i64 0, i64 %20
  store i32 %15, i32* %21, align 4
  br label %22

22:                                               ; preds = %13
  %23 = load i32, i32* %2, align 4
  %24 = add nsw i32 %23, 1
  store i32 %24, i32* %2, align 4
  br label %10, !llvm.loop !13

25:                                               ; preds = %10
  br label %26

26:                                               ; preds = %25
  %27 = load i32, i32* %1, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, i32* %1, align 4
  br label %6, !llvm.loop !14

29:                                               ; preds = %6
  store i32 0, i32* %3, align 4
  br label %30

30:                                               ; preds = %63, %29
  %31 = load i32, i32* %3, align 4
  %32 = icmp slt i32 %31, 10000
  br i1 %32, label %33, label %66

33:                                               ; preds = %30
  call void @_Z11CalcNextGenv()
  call void @_Z7DrawGenv()
  store i32 0, i32* %4, align 4
  br label %34

34:                                               ; preds = %59, %33
  %35 = load i32, i32* %4, align 4
  %36 = icmp slt i32 %35, 600
  br i1 %36, label %37, label %62

37:                                               ; preds = %34
  store i32 0, i32* %5, align 4
  br label %38

38:                                               ; preds = %55, %37
  %39 = load i32, i32* %5, align 4
  %40 = icmp slt i32 %39, 400
  br i1 %40, label %41, label %58

41:                                               ; preds = %38
  %42 = load i32, i32* %5, align 4
  %43 = mul nsw i32 %42, 600
  %44 = load i32, i32* %4, align 4
  %45 = add nsw i32 %43, %44
  %46 = sext i32 %45 to i64
  %47 = getelementptr inbounds [240000 x i32], [240000 x i32]* @next_gen, i64 0, i64 %46
  %48 = load i32, i32* %47, align 4
  %49 = load i32, i32* %5, align 4
  %50 = mul nsw i32 %49, 600
  %51 = load i32, i32* %4, align 4
  %52 = add nsw i32 %50, %51
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds [240000 x i32], [240000 x i32]* @current_gen, i64 0, i64 %53
  store i32 %48, i32* %54, align 4
  br label %55

55:                                               ; preds = %41
  %56 = load i32, i32* %5, align 4
  %57 = add nsw i32 %56, 1
  store i32 %57, i32* %5, align 4
  br label %38, !llvm.loop !15

58:                                               ; preds = %38
  br label %59

59:                                               ; preds = %58
  %60 = load i32, i32* %4, align 4
  %61 = add nsw i32 %60, 1
  store i32 %61, i32* %4, align 4
  br label %34, !llvm.loop !16

62:                                               ; preds = %34
  br label %63

63:                                               ; preds = %62
  %64 = load i32, i32* %3, align 4
  %65 = add nsw i32 %64, 1
  store i32 %65, i32* %3, align 4
  br label %30, !llvm.loop !17

66:                                               ; preds = %30
  ret void
}

declare noundef i32 @_Z7SimRandv() #1

attributes #0 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
!14 = distinct !{!14, !7}
!15 = distinct !{!15, !7}
!16 = distinct !{!16, !7}
!17 = distinct !{!17, !7}
