; ModuleID = 'probe1.7027f380-cgu.0'
source_filename = "probe1.7027f380-cgu.0"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc"

%"core::fmt::Arguments" = type { { ptr, i64 }, { ptr, i64 }, { ptr, i64 } }
%"alloc::string::String" = type { %"alloc::vec::Vec<u8>" }
%"alloc::vec::Vec<u8>" = type { { ptr, i64 }, i64 }
%"core::ptr::metadata::PtrComponents<u8>" = type { ptr, {} }
%"core::ptr::metadata::PtrRepr<u8>" = type { [1 x i64] }
%"core::ptr::metadata::PtrRepr<[u8]>" = type { [2 x i64] }
%"[closure@core::intrinsics::copy_nonoverlapping<u8>::{closure#0}]" = type { ptr, ptr, ptr }
%"alloc::alloc::Global" = type {}
%"core::option::Option<(core::ptr::non_null::NonNull<u8>, core::alloc::layout::Layout)>" = type { [2 x i64], i64 }

@alloc3 = private unnamed_addr constant <{}> zeroinitializer, align 8
@alloc9 = private unnamed_addr constant <{ [12 x i8] }> <{ [12 x i8] c"invalid args" }>, align 1
@alloc10 = private unnamed_addr constant <{ ptr, [8 x i8] }> <{ ptr @alloc9, [8 x i8] c"\0C\00\00\00\00\00\00\00" }>, align 8
@alloc83 = private unnamed_addr constant <{ [75 x i8] }> <{ [75 x i8] c"/rustc/bc4b39c271bbd36736cbf1c0a1ac23d5df38d365\\library\\core\\src\\fmt\\mod.rs" }>, align 1
@alloc84 = private unnamed_addr constant <{ ptr, [16 x i8] }> <{ ptr @alloc83, [16 x i8] c"K\00\00\00\00\00\00\00\8C\01\00\00\0D\00\00\00" }>, align 8
@alloc85 = private unnamed_addr constant <{ [80 x i8] }> <{ [80 x i8] c"/rustc/bc4b39c271bbd36736cbf1c0a1ac23d5df38d365\\library\\core\\src\\alloc\\layout.rs" }>, align 1
@alloc86 = private unnamed_addr constant <{ ptr, [16 x i8] }> <{ ptr @alloc85, [16 x i8] c"P\00\00\00\00\00\00\00\B4\01\00\00)\00\00\00" }>, align 8
@str.0 = internal constant [25 x i8] c"attempt to divide by zero"
@alloc87 = private unnamed_addr constant <{ [76 x i8] }> <{ [76 x i8] c"/rustc/bc4b39c271bbd36736cbf1c0a1ac23d5df38d365\\library\\alloc\\src\\raw_vec.rs" }>, align 1
@alloc88 = private unnamed_addr constant <{ ptr, [16 x i8] }> <{ ptr @alloc87, [16 x i8] c"L\00\00\00\00\00\00\00\F7\00\00\00;\00\00\00" }>, align 8
@alloc4 = private unnamed_addr constant <{ ptr, [8 x i8] }> <{ ptr @alloc3, [8 x i8] zeroinitializer }>, align 8
@alloc6 = private unnamed_addr constant <{ [8 x i8] }> zeroinitializer, align 8

; <core::ptr::non_null::NonNull<T> as core::convert::From<core::ptr::unique::Unique<T>>>::from
; Function Attrs: inlinehint uwtable
define ptr @"_ZN119_$LT$core..ptr..non_null..NonNull$LT$T$GT$$u20$as$u20$core..convert..From$LT$core..ptr..unique..Unique$LT$T$GT$$GT$$GT$4from17h132f2856fb93fa1fE"(ptr %unique) unnamed_addr #0 {
start:
  %0 = alloca ptr, align 8
  store ptr %unique, ptr %0, align 8
  %1 = load ptr, ptr %0, align 8, !nonnull !1, !noundef !1
  ret ptr %1
}

; core::fmt::ArgumentV1::new_lower_exp
; Function Attrs: inlinehint uwtable
define { ptr, ptr } @_ZN4core3fmt10ArgumentV113new_lower_exp17h31ae6fce06b32472E(ptr align 8 %x) unnamed_addr #0 {
start:
  %0 = alloca ptr, align 8
  %1 = alloca ptr, align 8
  %2 = alloca { ptr, ptr }, align 8
  store ptr @"_ZN4core3fmt3num3imp55_$LT$impl$u20$core..fmt..LowerExp$u20$for$u20$isize$GT$3fmt17h05ae56db2110b6c6E", ptr %1, align 8
  %_4 = load ptr, ptr %1, align 8, !nonnull !1, !noundef !1
  br label %bb1

bb1:                                              ; preds = %start
  store ptr %x, ptr %0, align 8
  %_6 = load ptr, ptr %0, align 8, !nonnull !1, !align !2, !noundef !1
  br label %bb2

bb2:                                              ; preds = %bb1
  store ptr %_6, ptr %2, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %2, i32 0, i32 1
  store ptr %_4, ptr %3, align 8
  %4 = getelementptr inbounds { ptr, ptr }, ptr %2, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8, !nonnull !1, !align !2, !noundef !1
  %6 = getelementptr inbounds { ptr, ptr }, ptr %2, i32 0, i32 1
  %7 = load ptr, ptr %6, align 8, !nonnull !1, !noundef !1
  %8 = insertvalue { ptr, ptr } undef, ptr %5, 0
  %9 = insertvalue { ptr, ptr } %8, ptr %7, 1
  ret { ptr, ptr } %9
}

; core::fmt::Arguments::as_str
; Function Attrs: inlinehint uwtable
define internal { ptr, i64 } @_ZN4core3fmt9Arguments6as_str17hfae9973a18645451E(ptr align 8 %self) unnamed_addr #0 {
start:
  %_2 = alloca { { ptr, i64 }, { ptr, i64 } }, align 8
  %0 = alloca { ptr, i64 }, align 8
  %1 = getelementptr inbounds { ptr, i64 }, ptr %self, i32 0, i32 0
  %_3.0 = load ptr, ptr %1, align 8, !nonnull !1, !align !3, !noundef !1
  %2 = getelementptr inbounds { ptr, i64 }, ptr %self, i32 0, i32 1
  %_3.1 = load i64, ptr %2, align 8
  %3 = getelementptr inbounds %"core::fmt::Arguments", ptr %self, i32 0, i32 2
  %4 = getelementptr inbounds { ptr, i64 }, ptr %3, i32 0, i32 0
  %_4.0 = load ptr, ptr %4, align 8, !nonnull !1, !align !3, !noundef !1
  %5 = getelementptr inbounds { ptr, i64 }, ptr %3, i32 0, i32 1
  %_4.1 = load i64, ptr %5, align 8
  %6 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 0
  store ptr %_3.0, ptr %6, align 8
  %7 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 1
  store i64 %_3.1, ptr %7, align 8
  %8 = getelementptr inbounds { { ptr, i64 }, { ptr, i64 } }, ptr %_2, i32 0, i32 1
  %9 = getelementptr inbounds { ptr, i64 }, ptr %8, i32 0, i32 0
  store ptr %_4.0, ptr %9, align 8
  %10 = getelementptr inbounds { ptr, i64 }, ptr %8, i32 0, i32 1
  store i64 %_4.1, ptr %10, align 8
  %11 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 0
  %_21.0 = load ptr, ptr %11, align 8, !nonnull !1, !align !3, !noundef !1
  %12 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 1
  %_21.1 = load i64, ptr %12, align 8
  %_16 = icmp eq i64 %_21.1, 0
  br i1 %_16, label %bb1, label %bb3

bb3:                                              ; preds = %start
  %13 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 0
  %_23.0 = load ptr, ptr %13, align 8, !nonnull !1, !align !3, !noundef !1
  %14 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 1
  %_23.1 = load i64, ptr %14, align 8
  %_13 = icmp eq i64 %_23.1, 1
  br i1 %_13, label %bb4, label %bb2

bb1:                                              ; preds = %start
  %15 = getelementptr inbounds { { ptr, i64 }, { ptr, i64 } }, ptr %_2, i32 0, i32 1
  %16 = getelementptr inbounds { ptr, i64 }, ptr %15, i32 0, i32 0
  %_22.0 = load ptr, ptr %16, align 8, !nonnull !1, !align !3, !noundef !1
  %17 = getelementptr inbounds { ptr, i64 }, ptr %15, i32 0, i32 1
  %_22.1 = load i64, ptr %17, align 8
  %_7 = icmp eq i64 %_22.1, 0
  br i1 %_7, label %bb5, label %bb2

bb2:                                              ; preds = %bb4, %bb3, %bb1
  store ptr null, ptr %0, align 8
  br label %bb7

bb5:                                              ; preds = %bb1
  %18 = getelementptr inbounds { ptr, i64 }, ptr %0, i32 0, i32 0
  store ptr @alloc3, ptr %18, align 8
  %19 = getelementptr inbounds { ptr, i64 }, ptr %0, i32 0, i32 1
  store i64 0, ptr %19, align 8
  br label %bb7

bb7:                                              ; preds = %bb2, %bb6, %bb5
  %20 = getelementptr inbounds { ptr, i64 }, ptr %0, i32 0, i32 0
  %21 = load ptr, ptr %20, align 8, !align !2
  %22 = getelementptr inbounds { ptr, i64 }, ptr %0, i32 0, i32 1
  %23 = load i64, ptr %22, align 8
  %24 = insertvalue { ptr, i64 } undef, ptr %21, 0
  %25 = insertvalue { ptr, i64 } %24, i64 %23, 1
  ret { ptr, i64 } %25

bb4:                                              ; preds = %bb3
  %26 = getelementptr inbounds { { ptr, i64 }, { ptr, i64 } }, ptr %_2, i32 0, i32 1
  %27 = getelementptr inbounds { ptr, i64 }, ptr %26, i32 0, i32 0
  %_24.0 = load ptr, ptr %27, align 8, !nonnull !1, !align !3, !noundef !1
  %28 = getelementptr inbounds { ptr, i64 }, ptr %26, i32 0, i32 1
  %_24.1 = load i64, ptr %28, align 8
  %_10 = icmp eq i64 %_24.1, 0
  br i1 %_10, label %bb6, label %bb2

bb6:                                              ; preds = %bb4
  %29 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 0
  %_25.0 = load ptr, ptr %29, align 8, !nonnull !1, !align !3, !noundef !1
  %30 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 1
  %_25.1 = load i64, ptr %30, align 8
  %s = getelementptr inbounds [0 x { ptr, i64 }], ptr %_25.0, i64 0, i64 0
  %31 = getelementptr inbounds { ptr, i64 }, ptr %s, i32 0, i32 0
  %_26.0 = load ptr, ptr %31, align 8, !nonnull !1, !align !2, !noundef !1
  %32 = getelementptr inbounds { ptr, i64 }, ptr %s, i32 0, i32 1
  %_26.1 = load i64, ptr %32, align 8
  %33 = getelementptr inbounds { ptr, i64 }, ptr %0, i32 0, i32 0
  store ptr %_26.0, ptr %33, align 8
  %34 = getelementptr inbounds { ptr, i64 }, ptr %0, i32 0, i32 1
  store i64 %_26.1, ptr %34, align 8
  br label %bb7
}

; core::fmt::Arguments::new_v1
; Function Attrs: inlinehint uwtable
define internal void @_ZN4core3fmt9Arguments6new_v117ha40a2220dbd1c6f3E(ptr sret(%"core::fmt::Arguments") %0, ptr align 8 %pieces.0, i64 %pieces.1, ptr align 8 %args.0, i64 %args.1) unnamed_addr #0 {
start:
  %_24 = alloca { ptr, i64 }, align 8
  %_16 = alloca %"core::fmt::Arguments", align 8
  %_3 = alloca i8, align 1
  %_4 = icmp ult i64 %pieces.1, %args.1
  br i1 %_4, label %bb1, label %bb2

bb2:                                              ; preds = %start
  %_12 = add i64 %args.1, 1
  %_9 = icmp ugt i64 %pieces.1, %_12
  %1 = zext i1 %_9 to i8
  store i8 %1, ptr %_3, align 1
  br label %bb3

bb1:                                              ; preds = %start
  store i8 1, ptr %_3, align 1
  br label %bb3

bb3:                                              ; preds = %bb2, %bb1
  %2 = load i8, ptr %_3, align 1, !range !4, !noundef !1
  %3 = trunc i8 %2 to i1
  br i1 %3, label %bb4, label %bb6

bb6:                                              ; preds = %bb3
  store ptr null, ptr %_24, align 8
  %4 = getelementptr inbounds { ptr, i64 }, ptr %0, i32 0, i32 0
  store ptr %pieces.0, ptr %4, align 8
  %5 = getelementptr inbounds { ptr, i64 }, ptr %0, i32 0, i32 1
  store i64 %pieces.1, ptr %5, align 8
  %6 = getelementptr inbounds %"core::fmt::Arguments", ptr %0, i32 0, i32 1
  %7 = getelementptr inbounds { ptr, i64 }, ptr %_24, i32 0, i32 0
  %8 = load ptr, ptr %7, align 8, !align !3
  %9 = getelementptr inbounds { ptr, i64 }, ptr %_24, i32 0, i32 1
  %10 = load i64, ptr %9, align 8
  %11 = getelementptr inbounds { ptr, i64 }, ptr %6, i32 0, i32 0
  store ptr %8, ptr %11, align 8
  %12 = getelementptr inbounds { ptr, i64 }, ptr %6, i32 0, i32 1
  store i64 %10, ptr %12, align 8
  %13 = getelementptr inbounds %"core::fmt::Arguments", ptr %0, i32 0, i32 2
  %14 = getelementptr inbounds { ptr, i64 }, ptr %13, i32 0, i32 0
  store ptr %args.0, ptr %14, align 8
  %15 = getelementptr inbounds { ptr, i64 }, ptr %13, i32 0, i32 1
  store i64 %args.1, ptr %15, align 8
  ret void

bb4:                                              ; preds = %bb3
; call core::fmt::Arguments::new_v1
  call void @_ZN4core3fmt9Arguments6new_v117ha40a2220dbd1c6f3E(ptr sret(%"core::fmt::Arguments") %_16, ptr align 8 @alloc10, i64 1, ptr align 8 @alloc3, i64 0)
  br label %bb5

bb5:                                              ; preds = %bb4
; call core::panicking::panic_fmt
  call void @_ZN4core9panicking9panic_fmt17hdcb6d460fcce0538E(ptr %_16, ptr align 8 @alloc84) #13
  unreachable
}

; core::ops::function::FnOnce::call_once
; Function Attrs: inlinehint uwtable
define internal void @_ZN4core3ops8function6FnOnce9call_once17h8d58108188dd5b1dE(ptr sret(%"alloc::string::String") %0, ptr align 1 %1, i64 %2) unnamed_addr #0 {
start:
  %_2 = alloca { ptr, i64 }, align 8
  %3 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 0
  store ptr %1, ptr %3, align 8
  %4 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 1
  store i64 %2, ptr %4, align 8
  %5 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 0
  %6 = load ptr, ptr %5, align 8, !nonnull !1, !align !2, !noundef !1
  %7 = getelementptr inbounds { ptr, i64 }, ptr %_2, i32 0, i32 1
  %8 = load i64, ptr %7, align 8
; call alloc::str::<impl alloc::borrow::ToOwned for str>::to_owned
  call void @"_ZN5alloc3str56_$LT$impl$u20$alloc..borrow..ToOwned$u20$for$u20$str$GT$8to_owned17hf60a7ede0950008fE"(ptr sret(%"alloc::string::String") %0, ptr align 1 %6, i64 %8)
  br label %bb1

bb1:                                              ; preds = %start
  ret void
}

; core::ptr::drop_in_place<alloc::string::String>
; Function Attrs: uwtable
define void @"_ZN4core3ptr42drop_in_place$LT$alloc..string..String$GT$17h1ae547631abb8825E"(ptr %_1) unnamed_addr #1 {
start:
; call core::ptr::drop_in_place<alloc::vec::Vec<u8>>
  call void @"_ZN4core3ptr46drop_in_place$LT$alloc..vec..Vec$LT$u8$GT$$GT$17h69c156b3f007e193E"(ptr %_1)
  br label %bb1

bb1:                                              ; preds = %start
  ret void
}

; core::ptr::drop_in_place<alloc::vec::Vec<u8>>
; Function Attrs: uwtable
define void @"_ZN4core3ptr46drop_in_place$LT$alloc..vec..Vec$LT$u8$GT$$GT$17h69c156b3f007e193E"(ptr %_1) unnamed_addr #1 personality ptr @__CxxFrameHandler3 {
start:
; invoke <alloc::vec::Vec<T,A> as core::ops::drop::Drop>::drop
  invoke void @"_ZN70_$LT$alloc..vec..Vec$LT$T$C$A$GT$$u20$as$u20$core..ops..drop..Drop$GT$4drop17hdcde2af413610940E"(ptr align 8 %_1)
          to label %bb4 unwind label %funclet_bb3

bb3:                                              ; preds = %funclet_bb3
; call core::ptr::drop_in_place<alloc::raw_vec::RawVec<u8>>
  call void @"_ZN4core3ptr53drop_in_place$LT$alloc..raw_vec..RawVec$LT$u8$GT$$GT$17h8b4428341baf4d9cE"(ptr %_1) #14 [ "funclet"(token %cleanuppad) ]
  br label %bb1

funclet_bb3:                                      ; preds = %start
  %cleanuppad = cleanuppad within none []
  br label %bb3

bb4:                                              ; preds = %start
; call core::ptr::drop_in_place<alloc::raw_vec::RawVec<u8>>
  call void @"_ZN4core3ptr53drop_in_place$LT$alloc..raw_vec..RawVec$LT$u8$GT$$GT$17h8b4428341baf4d9cE"(ptr %_1)
  br label %bb2

bb1:                                              ; preds = %bb3
  cleanupret from %cleanuppad unwind to caller

bb2:                                              ; preds = %bb4
  ret void
}

; core::ptr::drop_in_place<alloc::raw_vec::RawVec<u8>>
; Function Attrs: uwtable
define void @"_ZN4core3ptr53drop_in_place$LT$alloc..raw_vec..RawVec$LT$u8$GT$$GT$17h8b4428341baf4d9cE"(ptr %_1) unnamed_addr #1 {
start:
; call <alloc::raw_vec::RawVec<T,A> as core::ops::drop::Drop>::drop
  call void @"_ZN77_$LT$alloc..raw_vec..RawVec$LT$T$C$A$GT$$u20$as$u20$core..ops..drop..Drop$GT$4drop17h20d6bc0da571b7f6E"(ptr align 8 %_1)
  br label %bb1

bb1:                                              ; preds = %start
  ret void
}

; core::ptr::non_null::NonNull<T>::new
; Function Attrs: inlinehint uwtable
define ptr @"_ZN4core3ptr8non_null16NonNull$LT$T$GT$3new17h480d3af561d0e27eE"(ptr %ptr) unnamed_addr #0 {
start:
  %0 = alloca i8, align 1
  %1 = alloca ptr, align 8
  %_15 = alloca %"core::ptr::metadata::PtrComponents<u8>", align 8
  %_14 = alloca %"core::ptr::metadata::PtrRepr<u8>", align 8
  %_5 = alloca ptr, align 8
  %2 = alloca ptr, align 8
  store i64 0, ptr %1, align 8
  %data_address = load ptr, ptr %1, align 8
  br label %bb4

bb4:                                              ; preds = %start
  store ptr %data_address, ptr %_15, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %_14, ptr align 8 %_15, i64 8, i1 false)
  %other = load ptr, ptr %_14, align 8
  %3 = icmp eq ptr %ptr, %other
  %4 = zext i1 %3 to i8
  store i8 %4, ptr %0, align 1
  %5 = load i8, ptr %0, align 1, !range !4, !noundef !1
  %_3 = trunc i8 %5 to i1
  br label %bb5

bb5:                                              ; preds = %bb4
  %_2 = xor i1 %_3, true
  br i1 %_2, label %bb1, label %bb2

bb2:                                              ; preds = %bb5
  store ptr null, ptr %2, align 8
  br label %bb3

bb1:                                              ; preds = %bb5
  store ptr %ptr, ptr %_5, align 8
  %6 = load ptr, ptr %_5, align 8, !nonnull !1, !noundef !1
  store ptr %6, ptr %2, align 8
  br label %bb3

bb3:                                              ; preds = %bb2, %bb1
  %7 = load ptr, ptr %2, align 8
  ret ptr %7
}

; core::hint::unreachable_unchecked
; Function Attrs: inlinehint noreturn uwtable
define internal void @_ZN4core4hint21unreachable_unchecked17h31323ac447d15c0dE() unnamed_addr #2 {
start:
  unreachable
}

; core::alloc::layout::Layout::array::inner
; Function Attrs: inlinehint uwtable
define internal { i64, i64 } @_ZN4core5alloc6layout6Layout5array5inner17hd62e0f1362fcbab5E(i64 %element_size, i64 %align, i64 %n) unnamed_addr #0 {
start:
  %0 = alloca i64, align 8
  %self1 = alloca i64, align 8
  %self = alloca i64, align 8
  %_16 = alloca { i64, i64 }, align 8
  %_4 = alloca i8, align 1
  %1 = alloca { i64, i64 }, align 8
  %2 = icmp eq i64 %element_size, 0
  br i1 %2, label %bb1, label %bb2

bb1:                                              ; preds = %start
  store i8 0, ptr %_4, align 1
  br label %bb3

bb2:                                              ; preds = %start
  store i64 %align, ptr %self1, align 8
  %_24 = load i64, ptr %self1, align 8, !range !5, !noundef !1
  %_21 = sub i64 %_24, 1
  %_9 = sub i64 9223372036854775807, %_21
  %_12 = icmp eq i64 %element_size, 0
  %3 = call i1 @llvm.expect.i1(i1 %_12, i1 false)
  br i1 %3, label %panic, label %bb4

bb4:                                              ; preds = %bb2
  %_8 = udiv i64 %_9, %element_size
  %_6 = icmp ugt i64 %n, %_8
  %4 = zext i1 %_6 to i8
  store i8 %4, ptr %_4, align 1
  br label %bb3

panic:                                            ; preds = %bb2
; call core::panicking::panic
  call void @_ZN4core9panicking5panic17h461763e65d8957b4E(ptr align 1 @str.0, i64 25, ptr align 8 @alloc86) #13
  unreachable

bb3:                                              ; preds = %bb1, %bb4
  %5 = load i8, ptr %_4, align 1, !range !4, !noundef !1
  %6 = trunc i8 %5 to i1
  br i1 %6, label %bb5, label %bb6

bb6:                                              ; preds = %bb3
  %array_size = mul i64 %element_size, %n
  store i64 %align, ptr %self, align 8
  %_25 = load i64, ptr %self, align 8, !range !5, !noundef !1
  store i64 %_25, ptr %0, align 8
  %_27 = load i64, ptr %0, align 8, !range !5, !noundef !1
  br label %bb8

bb5:                                              ; preds = %bb3
  %7 = getelementptr inbounds { i64, i64 }, ptr %1, i32 0, i32 1
  store i64 0, ptr %7, align 8
  br label %bb7

bb7:                                              ; preds = %bb8, %bb5
  %8 = getelementptr inbounds { i64, i64 }, ptr %1, i32 0, i32 0
  %9 = load i64, ptr %8, align 8
  %10 = getelementptr inbounds { i64, i64 }, ptr %1, i32 0, i32 1
  %11 = load i64, ptr %10, align 8, !range !6, !noundef !1
  %12 = insertvalue { i64, i64 } undef, i64 %9, 0
  %13 = insertvalue { i64, i64 } %12, i64 %11, 1
  ret { i64, i64 } %13

bb8:                                              ; preds = %bb6
  store i64 %array_size, ptr %_16, align 8
  %14 = getelementptr inbounds { i64, i64 }, ptr %_16, i32 0, i32 1
  store i64 %_27, ptr %14, align 8
  %15 = getelementptr inbounds { i64, i64 }, ptr %_16, i32 0, i32 0
  %16 = load i64, ptr %15, align 8
  %17 = getelementptr inbounds { i64, i64 }, ptr %_16, i32 0, i32 1
  %18 = load i64, ptr %17, align 8, !range !5, !noundef !1
  %19 = getelementptr inbounds { i64, i64 }, ptr %1, i32 0, i32 0
  store i64 %16, ptr %19, align 8
  %20 = getelementptr inbounds { i64, i64 }, ptr %1, i32 0, i32 1
  store i64 %18, ptr %20, align 8
  br label %bb7
}

; core::option::Option<T>::map_or_else
; Function Attrs: inlinehint uwtable
define void @"_ZN4core6option15Option$LT$T$GT$11map_or_else17h74778ece6b2b18d2E"(ptr sret(%"alloc::string::String") %0, ptr align 1 %1, i64 %2, ptr align 8 %default) unnamed_addr #0 personality ptr @__CxxFrameHandler3 {
start:
  %_12 = alloca i8, align 1
  %_11 = alloca i8, align 1
  %_7 = alloca { ptr, i64 }, align 8
  %self = alloca { ptr, i64 }, align 8
  %3 = getelementptr inbounds { ptr, i64 }, ptr %self, i32 0, i32 0
  store ptr %1, ptr %3, align 8
  %4 = getelementptr inbounds { ptr, i64 }, ptr %self, i32 0, i32 1
  store i64 %2, ptr %4, align 8
  store i8 1, ptr %_12, align 1
  store i8 1, ptr %_11, align 1
  %5 = load ptr, ptr %self, align 8
  %6 = icmp eq ptr %5, null
  %_4 = select i1 %6, i64 0, i64 1
  switch i64 %_4, label %bb2 [
    i64 0, label %bb1
    i64 1, label %bb3
  ]

bb2:                                              ; preds = %start
  unreachable

bb1:                                              ; preds = %start
  store i8 0, ptr %_12, align 1
; invoke alloc::fmt::format::{{closure}}
  invoke void @"_ZN5alloc3fmt6format28_$u7b$$u7b$closure$u7d$$u7d$17h54e889c780bc9d6eE"(ptr sret(%"alloc::string::String") %0, ptr align 8 %default)
          to label %bb5 unwind label %funclet_bb14

bb3:                                              ; preds = %start
  %7 = getelementptr inbounds { ptr, i64 }, ptr %self, i32 0, i32 0
  %t.0 = load ptr, ptr %7, align 8, !nonnull !1, !align !2, !noundef !1
  %8 = getelementptr inbounds { ptr, i64 }, ptr %self, i32 0, i32 1
  %t.1 = load i64, ptr %8, align 8
  store i8 0, ptr %_11, align 1
  %9 = getelementptr inbounds { ptr, i64 }, ptr %_7, i32 0, i32 0
  store ptr %t.0, ptr %9, align 8
  %10 = getelementptr inbounds { ptr, i64 }, ptr %_7, i32 0, i32 1
  store i64 %t.1, ptr %10, align 8
  %11 = getelementptr inbounds { ptr, i64 }, ptr %_7, i32 0, i32 0
  %12 = load ptr, ptr %11, align 8, !nonnull !1, !align !2, !noundef !1
  %13 = getelementptr inbounds { ptr, i64 }, ptr %_7, i32 0, i32 1
  %14 = load i64, ptr %13, align 8
; invoke core::ops::function::FnOnce::call_once
  invoke void @_ZN4core3ops8function6FnOnce9call_once17h8d58108188dd5b1dE(ptr sret(%"alloc::string::String") %0, ptr align 1 %12, i64 %14)
          to label %bb4 unwind label %funclet_bb14

bb14:                                             ; preds = %funclet_bb14
  %15 = load i8, ptr %_11, align 1, !range !4, !noundef !1
  %16 = trunc i8 %15 to i1
  br i1 %16, label %bb13, label %bb14_cleanup_trampoline_bb8

funclet_bb14:                                     ; preds = %bb1, %bb3
  %cleanuppad = cleanuppad within none []
  br label %bb14

bb4:                                              ; preds = %bb3
  br label %bb11

bb11:                                             ; preds = %bb5, %bb4
  %17 = load i8, ptr %_11, align 1, !range !4, !noundef !1
  %18 = trunc i8 %17 to i1
  br i1 %18, label %bb10, label %bb6

bb5:                                              ; preds = %bb1
  br label %bb11

bb8:                                              ; preds = %funclet_bb8
  %19 = load i8, ptr %_12, align 1, !range !4, !noundef !1
  %20 = trunc i8 %19 to i1
  br i1 %20, label %bb15, label %bb9

funclet_bb8:                                      ; preds = %bb13, %bb14_cleanup_trampoline_bb8
  %cleanuppad1 = cleanuppad within none []
  br label %bb8

bb14_cleanup_trampoline_bb8:                      ; preds = %bb14
  cleanupret from %cleanuppad unwind label %funclet_bb8

bb13:                                             ; preds = %bb14
  cleanupret from %cleanuppad unwind label %funclet_bb8

bb6:                                              ; preds = %bb10, %bb11
  %21 = load i8, ptr %_12, align 1, !range !4, !noundef !1
  %22 = trunc i8 %21 to i1
  br i1 %22, label %bb12, label %bb7

bb10:                                             ; preds = %bb11
  br label %bb6

bb9:                                              ; preds = %bb15, %bb8
  cleanupret from %cleanuppad1 unwind to caller

bb15:                                             ; preds = %bb8
  br label %bb9

bb7:                                              ; preds = %bb12, %bb6
  ret void

bb12:                                             ; preds = %bb6
  br label %bb7
}

; core::result::Result<T,E>::unwrap_unchecked
; Function Attrs: inlinehint uwtable
define { i64, i64 } @"_ZN4core6result19Result$LT$T$C$E$GT$16unwrap_unchecked17h8f0c2699931e05fdE"(i64 %0, i64 %1, ptr align 8 %2) unnamed_addr #0 personality ptr @__CxxFrameHandler3 {
start:
  %self = alloca { i64, i64 }, align 8
  %3 = getelementptr inbounds { i64, i64 }, ptr %self, i32 0, i32 0
  store i64 %0, ptr %3, align 8
  %4 = getelementptr inbounds { i64, i64 }, ptr %self, i32 0, i32 1
  store i64 %1, ptr %4, align 8
  %5 = getelementptr inbounds { i64, i64 }, ptr %self, i32 0, i32 1
  %6 = load i64, ptr %5, align 8, !range !6, !noundef !1
  %7 = icmp eq i64 %6, 0
  %_3 = select i1 %7, i64 1, i64 0
  switch i64 %_3, label %bb2 [
    i64 0, label %bb3
    i64 1, label %bb1
  ]

bb2:                                              ; preds = %start
  unreachable

bb3:                                              ; preds = %start
  %8 = getelementptr inbounds { i64, i64 }, ptr %self, i32 0, i32 0
  %t.0 = load i64, ptr %8, align 8
  %9 = getelementptr inbounds { i64, i64 }, ptr %self, i32 0, i32 1
  %t.1 = load i64, ptr %9, align 8, !range !5, !noundef !1
  %10 = getelementptr inbounds { i64, i64 }, ptr %self, i32 0, i32 1
  %11 = load i64, ptr %10, align 8, !range !6, !noundef !1
  %12 = icmp eq i64 %11, 0
  %_7 = select i1 %12, i64 1, i64 0
  %13 = icmp eq i64 %_7, 0
  br i1 %13, label %bb5, label %bb6

bb1:                                              ; preds = %start
; invoke core::hint::unreachable_unchecked
  invoke void @_ZN4core4hint21unreachable_unchecked17h31323ac447d15c0dE() #13
          to label %unreachable unwind label %funclet_bb10

bb10:                                             ; preds = %funclet_bb10
  %14 = getelementptr inbounds { i64, i64 }, ptr %self, i32 0, i32 1
  %15 = load i64, ptr %14, align 8, !range !6, !noundef !1
  %16 = icmp eq i64 %15, 0
  %_8 = select i1 %16, i64 1, i64 0
  %17 = icmp eq i64 %_8, 0
  br i1 %17, label %bb7, label %bb9

funclet_bb10:                                     ; preds = %bb1
  %cleanuppad = cleanuppad within none []
  br label %bb10

unreachable:                                      ; preds = %bb1
  unreachable

bb7:                                              ; preds = %bb10
  br i1 true, label %bb8, label %bb4

bb9:                                              ; preds = %bb10
  br label %bb4

bb4:                                              ; preds = %bb8, %bb7, %bb9
  cleanupret from %cleanuppad unwind to caller

bb8:                                              ; preds = %bb7
  br label %bb4

bb5:                                              ; preds = %bb6, %bb3
  %18 = insertvalue { i64, i64 } undef, i64 %t.0, 0
  %19 = insertvalue { i64, i64 } %18, i64 %t.1, 1
  ret { i64, i64 } %19

bb6:                                              ; preds = %bb3
  br label %bb5
}

; <T as core::convert::From<T>>::from
; Function Attrs: uwtable
define void @"_ZN50_$LT$T$u20$as$u20$core..convert..From$LT$T$GT$$GT$4from17h7b1b4bdd72c70767E"() unnamed_addr #1 {
start:
  ret void
}

; <T as core::convert::Into<U>>::into
; Function Attrs: uwtable
define ptr @"_ZN50_$LT$T$u20$as$u20$core..convert..Into$LT$U$GT$$GT$4into17h4121e13b91be4aecE"(ptr %self) unnamed_addr #1 {
start:
; call <core::ptr::non_null::NonNull<T> as core::convert::From<core::ptr::unique::Unique<T>>>::from
  %0 = call ptr @"_ZN119_$LT$core..ptr..non_null..NonNull$LT$T$GT$$u20$as$u20$core..convert..From$LT$core..ptr..unique..Unique$LT$T$GT$$GT$$GT$4from17h132f2856fb93fa1fE"(ptr %self)
  br label %bb1

bb1:                                              ; preds = %start
  ret ptr %0
}

; <T as alloc::slice::hack::ConvertVec>::to_vec
; Function Attrs: inlinehint uwtable
define void @"_ZN52_$LT$T$u20$as$u20$alloc..slice..hack..ConvertVec$GT$6to_vec17he4e409077b38c2b0E"(ptr sret(%"alloc::vec::Vec<u8>") %v, ptr align 1 %s.0, i64 %s.1) unnamed_addr #0 personality ptr @__CxxFrameHandler3 {
start:
  %_34 = alloca %"core::ptr::metadata::PtrRepr<[u8]>", align 8
  %runtime = alloca %"[closure@core::intrinsics::copy_nonoverlapping<u8>::{closure#0}]", align 8
  %_25 = alloca %"core::ptr::metadata::PtrRepr<[u8]>", align 8
  %_22 = alloca i8, align 1
  %_16 = alloca %"core::ptr::metadata::PtrRepr<[u8]>", align 8
  %0 = getelementptr inbounds { ptr, i64 }, ptr %_16, i32 0, i32 0
  store ptr %s.0, ptr %0, align 8
  %1 = getelementptr inbounds { ptr, i64 }, ptr %_16, i32 0, i32 1
  store i64 %s.1, ptr %1, align 8
  %2 = getelementptr inbounds { ptr, i64 }, ptr %_16, i32 0, i32 1
  %capacity = load i64, ptr %2, align 8
  store i8 0, ptr %_22, align 1
  %3 = load i8, ptr %_22, align 1, !range !4, !noundef !1
  %4 = trunc i8 %3 to i1
; invoke alloc::raw_vec::RawVec<T,A>::allocate_in
  %5 = invoke { ptr, i64 } @"_ZN5alloc7raw_vec19RawVec$LT$T$C$A$GT$11allocate_in17h29d1eec2f7a42a7bE"(i64 %capacity, i1 zeroext %4)
          to label %bb6 unwind label %funclet_bb5

bb5:                                              ; preds = %funclet_bb5
  br i1 false, label %bb4, label %bb3

funclet_bb5:                                      ; preds = %bb2, %start
  %cleanuppad = cleanuppad within none []
  br label %bb5

bb6:                                              ; preds = %start
  %_18.0 = extractvalue { ptr, i64 } %5, 0
  %_18.1 = extractvalue { ptr, i64 } %5, 1
  %6 = getelementptr inbounds { ptr, i64 }, ptr %v, i32 0, i32 0
  store ptr %_18.0, ptr %6, align 8
  %7 = getelementptr inbounds { ptr, i64 }, ptr %v, i32 0, i32 1
  store i64 %_18.1, ptr %7, align 8
  %8 = getelementptr inbounds %"alloc::vec::Vec<u8>", ptr %v, i32 0, i32 1
  store i64 0, ptr %8, align 8
; invoke alloc::vec::Vec<T,A>::as_mut_ptr
  %dest = invoke ptr @"_ZN5alloc3vec16Vec$LT$T$C$A$GT$10as_mut_ptr17h546501678f6b91ebE"(ptr align 8 %v)
          to label %bb1 unwind label %funclet_bb2

bb2:                                              ; preds = %funclet_bb2
; call core::ptr::drop_in_place<alloc::vec::Vec<u8>>
  call void @"_ZN4core3ptr46drop_in_place$LT$alloc..vec..Vec$LT$u8$GT$$GT$17h69c156b3f007e193E"(ptr %v) #14 [ "funclet"(token %cleanuppad1) ]
  cleanupret from %cleanuppad1 unwind label %funclet_bb5

funclet_bb2:                                      ; preds = %bb6
  %cleanuppad1 = cleanuppad within none []
  br label %bb2

bb1:                                              ; preds = %bb6
  %9 = getelementptr inbounds { ptr, i64 }, ptr %_25, i32 0, i32 0
  store ptr %s.0, ptr %9, align 8
  %10 = getelementptr inbounds { ptr, i64 }, ptr %_25, i32 0, i32 1
  store i64 %s.1, ptr %10, align 8
  %11 = getelementptr inbounds { ptr, i64 }, ptr %_25, i32 0, i32 1
  %count = load i64, ptr %11, align 8
  %12 = mul i64 %count, 1
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %dest, ptr align 1 %s.0, i64 %12, i1 false)
  %13 = getelementptr inbounds { ptr, i64 }, ptr %_34, i32 0, i32 0
  store ptr %s.0, ptr %13, align 8
  %14 = getelementptr inbounds { ptr, i64 }, ptr %_34, i32 0, i32 1
  store i64 %s.1, ptr %14, align 8
  %15 = getelementptr inbounds { ptr, i64 }, ptr %_34, i32 0, i32 1
  %new_len = load i64, ptr %15, align 8
  %16 = getelementptr inbounds %"alloc::vec::Vec<u8>", ptr %v, i32 0, i32 1
  store i64 %new_len, ptr %16, align 8
  ret void

bb3:                                              ; preds = %bb4, %bb5
  cleanupret from %cleanuppad unwind to caller

bb4:                                              ; preds = %bb5
  br label %bb3
}

; alloc::fmt::format
; Function Attrs: inlinehint uwtable
define internal void @_ZN5alloc3fmt6format17hdd81d5776ef6c6b2E(ptr sret(%"alloc::string::String") %0, ptr %args) unnamed_addr #0 {
start:
  %_4 = alloca ptr, align 8
; call core::fmt::Arguments::as_str
  %1 = call { ptr, i64 } @_ZN4core3fmt9Arguments6as_str17hfae9973a18645451E(ptr align 8 %args)
  %_2.0 = extractvalue { ptr, i64 } %1, 0
  %_2.1 = extractvalue { ptr, i64 } %1, 1
  br label %bb1

bb1:                                              ; preds = %start
  store ptr %args, ptr %_4, align 8
  %2 = load ptr, ptr %_4, align 8, !nonnull !1, !align !3, !noundef !1
; call core::option::Option<T>::map_or_else
  call void @"_ZN4core6option15Option$LT$T$GT$11map_or_else17h74778ece6b2b18d2E"(ptr sret(%"alloc::string::String") %0, ptr align 1 %_2.0, i64 %_2.1, ptr align 8 %2)
  br label %bb2

bb2:                                              ; preds = %bb1
  ret void
}

; alloc::fmt::format::{{closure}}
; Function Attrs: inlinehint uwtable
define void @"_ZN5alloc3fmt6format28_$u7b$$u7b$closure$u7d$$u7d$17h54e889c780bc9d6eE"(ptr sret(%"alloc::string::String") %0, ptr align 8 %1) unnamed_addr #0 {
start:
  %_2 = alloca %"core::fmt::Arguments", align 8
  %_1 = alloca ptr, align 8
  store ptr %1, ptr %_1, align 8
  %_3 = load ptr, ptr %_1, align 8, !nonnull !1, !align !3, !noundef !1
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %_2, ptr align 8 %_3, i64 48, i1 false)
; call alloc::fmt::format::format_inner
  call void @_ZN5alloc3fmt6format12format_inner17hcaab02f56890c3e4E(ptr sret(%"alloc::string::String") %0, ptr %_2)
  br label %bb1

bb1:                                              ; preds = %start
  ret void
}

; alloc::str::<impl alloc::borrow::ToOwned for str>::to_owned
; Function Attrs: inlinehint uwtable
define internal void @"_ZN5alloc3str56_$LT$impl$u20$alloc..borrow..ToOwned$u20$for$u20$str$GT$8to_owned17hf60a7ede0950008fE"(ptr sret(%"alloc::string::String") %0, ptr align 1 %self.0, i64 %self.1) unnamed_addr #0 {
start:
  %1 = alloca { ptr, i64 }, align 8
  %_7 = alloca %"alloc::vec::Vec<u8>", align 8
  %bytes = alloca %"alloc::vec::Vec<u8>", align 8
  %2 = getelementptr inbounds { ptr, i64 }, ptr %1, i32 0, i32 0
  store ptr %self.0, ptr %2, align 8
  %3 = getelementptr inbounds { ptr, i64 }, ptr %1, i32 0, i32 1
  store i64 %self.1, ptr %3, align 8
  %4 = getelementptr inbounds { ptr, i64 }, ptr %1, i32 0, i32 0
  %_4.0 = load ptr, ptr %4, align 8, !nonnull !1, !align !2, !noundef !1
  %5 = getelementptr inbounds { ptr, i64 }, ptr %1, i32 0, i32 1
  %_4.1 = load i64, ptr %5, align 8
  br label %bb2

bb2:                                              ; preds = %start
; call alloc::slice::<impl alloc::borrow::ToOwned for [T]>::to_owned
  call void @"_ZN5alloc5slice64_$LT$impl$u20$alloc..borrow..ToOwned$u20$for$u20$$u5b$T$u5d$$GT$8to_owned17hb6043afda9e821aeE"(ptr sret(%"alloc::vec::Vec<u8>") %bytes, ptr align 1 %_4.0, i64 %_4.1)
  br label %bb1

bb1:                                              ; preds = %bb2
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %_7, ptr align 8 %bytes, i64 24, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %0, ptr align 8 %_7, i64 24, i1 false)
  ret void
}

; alloc::vec::Vec<T,A>::as_mut_ptr
; Function Attrs: inlinehint uwtable
define ptr @"_ZN5alloc3vec16Vec$LT$T$C$A$GT$10as_mut_ptr17h546501678f6b91ebE"(ptr align 8 %self) unnamed_addr #0 {
start:
  %0 = alloca i8, align 1
  %1 = alloca ptr, align 8
  %_18 = alloca %"core::ptr::metadata::PtrComponents<u8>", align 8
  %_17 = alloca %"core::ptr::metadata::PtrRepr<u8>", align 8
  %self1 = load ptr, ptr %self, align 8, !nonnull !1, !noundef !1
  store i64 0, ptr %1, align 8
  %data_address = load ptr, ptr %1, align 8
  br label %bb2

bb2:                                              ; preds = %start
  store ptr %data_address, ptr %_18, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %_17, ptr align 8 %_18, i64 8, i1 false)
  %other = load ptr, ptr %_17, align 8
  %2 = icmp eq ptr %self1, %other
  %3 = zext i1 %2 to i8
  store i8 %3, ptr %0, align 1
  %4 = load i8, ptr %0, align 1, !range !4, !noundef !1
  %_5 = trunc i8 %4 to i1
  br label %bb3

bb3:                                              ; preds = %bb2
  %_4 = xor i1 %_5, true
  call void @llvm.assume(i1 %_4)
  br label %bb1

bb1:                                              ; preds = %bb3
  ret ptr %self1
}

; alloc::alloc::Global::alloc_impl
; Function Attrs: inlinehint uwtable
define internal { ptr, i64 } @_ZN5alloc5alloc6Global10alloc_impl17h3e29304841a7fef4E(ptr align 1 %self, i64 %0, i64 %1, i1 zeroext %zeroed) unnamed_addr #0 {
start:
  %2 = alloca ptr, align 8
  %_82 = alloca { ptr, i64 }, align 8
  %_81 = alloca %"core::ptr::metadata::PtrRepr<[u8]>", align 8
  %self7 = alloca i64, align 8
  %self6 = alloca i64, align 8
  %_47 = alloca { ptr, i64 }, align 8
  %_46 = alloca %"core::ptr::metadata::PtrRepr<[u8]>", align 8
  %self5 = alloca i64, align 8
  %_24 = alloca { ptr, i64 }, align 8
  %self4 = alloca ptr, align 8
  %self3 = alloca ptr, align 8
  %_15 = alloca ptr, align 8
  %layout2 = alloca { i64, i64 }, align 8
  %layout1 = alloca { i64, i64 }, align 8
  %raw_ptr = alloca ptr, align 8
  %data = alloca ptr, align 8
  %_6 = alloca { ptr, i64 }, align 8
  %3 = alloca { ptr, i64 }, align 8
  %layout = alloca { i64, i64 }, align 8
  %4 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 0
  store i64 %0, ptr %4, align 8
  %5 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 1
  store i64 %1, ptr %5, align 8
  %_4 = load i64, ptr %layout, align 8
  %6 = icmp eq i64 %_4, 0
  br i1 %6, label %bb2, label %bb1

bb2:                                              ; preds = %start
  %7 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 1
  %8 = load i64, ptr %7, align 8, !range !5, !noundef !1
  store i64 %8, ptr %self5, align 8
  %_31 = load i64, ptr %self5, align 8, !range !5, !noundef !1
  store i64 %_31, ptr %2, align 8
  %ptr = load ptr, ptr %2, align 8
  br label %bb13

bb1:                                              ; preds = %start
  br i1 %zeroed, label %bb3, label %bb4

bb4:                                              ; preds = %bb1
  %9 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 0
  %10 = load i64, ptr %9, align 8
  %11 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 1
  %12 = load i64, ptr %11, align 8, !range !5, !noundef !1
  %13 = getelementptr inbounds { i64, i64 }, ptr %layout2, i32 0, i32 0
  store i64 %10, ptr %13, align 8
  %14 = getelementptr inbounds { i64, i64 }, ptr %layout2, i32 0, i32 1
  store i64 %12, ptr %14, align 8
  %_59 = load i64, ptr %layout2, align 8
  %15 = getelementptr inbounds { i64, i64 }, ptr %layout2, i32 0, i32 1
  %16 = load i64, ptr %15, align 8, !range !5, !noundef !1
  store i64 %16, ptr %self7, align 8
  %_64 = load i64, ptr %self7, align 8, !range !5, !noundef !1
  %17 = call ptr @__rust_alloc(i64 %_59, i64 %_64) #15
  store ptr %17, ptr %raw_ptr, align 8
  br label %bb15

bb3:                                              ; preds = %bb1
  %18 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 0
  %19 = load i64, ptr %18, align 8
  %20 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 1
  %21 = load i64, ptr %20, align 8, !range !5, !noundef !1
  %22 = getelementptr inbounds { i64, i64 }, ptr %layout1, i32 0, i32 0
  store i64 %19, ptr %22, align 8
  %23 = getelementptr inbounds { i64, i64 }, ptr %layout1, i32 0, i32 1
  store i64 %21, ptr %23, align 8
  %_53 = load i64, ptr %layout1, align 8
  %24 = getelementptr inbounds { i64, i64 }, ptr %layout1, i32 0, i32 1
  %25 = load i64, ptr %24, align 8, !range !5, !noundef !1
  store i64 %25, ptr %self6, align 8
  %_58 = load i64, ptr %self6, align 8, !range !5, !noundef !1
  %26 = call ptr @__rust_alloc_zeroed(i64 %_53, i64 %_58) #15
  store ptr %26, ptr %raw_ptr, align 8
  br label %bb14

bb14:                                             ; preds = %bb3
  br label %bb5

bb5:                                              ; preds = %bb15, %bb14
  %_18 = load ptr, ptr %raw_ptr, align 8
; call core::ptr::non_null::NonNull<T>::new
  %27 = call ptr @"_ZN4core3ptr8non_null16NonNull$LT$T$GT$3new17h480d3af561d0e27eE"(ptr %_18)
  store ptr %27, ptr %self4, align 8
  br label %bb6

bb15:                                             ; preds = %bb4
  br label %bb5

bb6:                                              ; preds = %bb5
  %28 = load ptr, ptr %self4, align 8
  %29 = icmp eq ptr %28, null
  %_65 = select i1 %29, i64 0, i64 1
  switch i64 %_65, label %bb17 [
    i64 0, label %bb16
    i64 1, label %bb18
  ]

bb17:                                             ; preds = %bb6
  unreachable

bb16:                                             ; preds = %bb6
  store ptr null, ptr %self3, align 8
  br label %bb19

bb18:                                             ; preds = %bb6
  %v = load ptr, ptr %self4, align 8, !nonnull !1, !noundef !1
  store ptr %v, ptr %self3, align 8
  br label %bb19

bb19:                                             ; preds = %bb16, %bb18
  %30 = load ptr, ptr %self3, align 8
  %31 = icmp eq ptr %30, null
  %_68 = select i1 %31, i64 1, i64 0
  switch i64 %_68, label %bb21 [
    i64 0, label %bb22
    i64 1, label %bb20
  ]

bb21:                                             ; preds = %bb19
  unreachable

bb22:                                             ; preds = %bb19
  %v8 = load ptr, ptr %self3, align 8, !nonnull !1, !noundef !1
  store ptr %v8, ptr %_15, align 8
  br label %bb7

bb20:                                             ; preds = %bb19
  store ptr null, ptr %_15, align 8
  br label %bb7

bb7:                                              ; preds = %bb22, %bb20
  %32 = load ptr, ptr %_15, align 8
  %33 = icmp eq ptr %32, null
  %_20 = select i1 %33, i64 1, i64 0
  switch i64 %_20, label %bb9 [
    i64 0, label %bb8
    i64 1, label %bb10
  ]

bb9:                                              ; preds = %bb7
  unreachable

bb8:                                              ; preds = %bb7
  %val = load ptr, ptr %_15, align 8, !nonnull !1, !noundef !1
  store ptr %val, ptr %_82, align 8
  %34 = getelementptr inbounds { ptr, i64 }, ptr %_82, i32 0, i32 1
  store i64 %_4, ptr %34, align 8
  %35 = getelementptr inbounds { ptr, i64 }, ptr %_82, i32 0, i32 0
  %36 = load ptr, ptr %35, align 8
  %37 = getelementptr inbounds { ptr, i64 }, ptr %_82, i32 0, i32 1
  %38 = load i64, ptr %37, align 8
  %39 = getelementptr inbounds { ptr, i64 }, ptr %_81, i32 0, i32 0
  store ptr %36, ptr %39, align 8
  %40 = getelementptr inbounds { ptr, i64 }, ptr %_81, i32 0, i32 1
  store i64 %38, ptr %40, align 8
  %41 = getelementptr inbounds { ptr, i64 }, ptr %_81, i32 0, i32 0
  %ptr.0 = load ptr, ptr %41, align 8
  %42 = getelementptr inbounds { ptr, i64 }, ptr %_81, i32 0, i32 1
  %ptr.1 = load i64, ptr %42, align 8
  %43 = getelementptr inbounds { ptr, i64 }, ptr %_24, i32 0, i32 0
  store ptr %ptr.0, ptr %43, align 8
  %44 = getelementptr inbounds { ptr, i64 }, ptr %_24, i32 0, i32 1
  store i64 %ptr.1, ptr %44, align 8
  %45 = getelementptr inbounds { ptr, i64 }, ptr %_24, i32 0, i32 0
  %46 = load ptr, ptr %45, align 8, !nonnull !1, !noundef !1
  %47 = getelementptr inbounds { ptr, i64 }, ptr %_24, i32 0, i32 1
  %48 = load i64, ptr %47, align 8
  %49 = getelementptr inbounds { ptr, i64 }, ptr %3, i32 0, i32 0
  store ptr %46, ptr %49, align 8
  %50 = getelementptr inbounds { ptr, i64 }, ptr %3, i32 0, i32 1
  store i64 %48, ptr %50, align 8
  br label %bb11

bb10:                                             ; preds = %bb7
; call <T as core::convert::From<T>>::from
  call void @"_ZN50_$LT$T$u20$as$u20$core..convert..From$LT$T$GT$$GT$4from17h7b1b4bdd72c70767E"()
  br label %bb23

bb23:                                             ; preds = %bb10
  store ptr null, ptr %3, align 8
  br label %bb12

bb12:                                             ; preds = %bb11, %bb23
  %51 = getelementptr inbounds { ptr, i64 }, ptr %3, i32 0, i32 0
  %52 = load ptr, ptr %51, align 8
  %53 = getelementptr inbounds { ptr, i64 }, ptr %3, i32 0, i32 1
  %54 = load i64, ptr %53, align 8
  %55 = insertvalue { ptr, i64 } undef, ptr %52, 0
  %56 = insertvalue { ptr, i64 } %55, i64 %54, 1
  ret { ptr, i64 } %56

bb11:                                             ; preds = %bb13, %bb8
  br label %bb12

bb13:                                             ; preds = %bb2
  store ptr %ptr, ptr %data, align 8
  %self9 = load ptr, ptr %data, align 8, !nonnull !1, !noundef !1
  store ptr %self9, ptr %_47, align 8
  %57 = getelementptr inbounds { ptr, i64 }, ptr %_47, i32 0, i32 1
  store i64 0, ptr %57, align 8
  %58 = getelementptr inbounds { ptr, i64 }, ptr %_47, i32 0, i32 0
  %59 = load ptr, ptr %58, align 8
  %60 = getelementptr inbounds { ptr, i64 }, ptr %_47, i32 0, i32 1
  %61 = load i64, ptr %60, align 8
  %62 = getelementptr inbounds { ptr, i64 }, ptr %_46, i32 0, i32 0
  store ptr %59, ptr %62, align 8
  %63 = getelementptr inbounds { ptr, i64 }, ptr %_46, i32 0, i32 1
  store i64 %61, ptr %63, align 8
  %64 = getelementptr inbounds { ptr, i64 }, ptr %_46, i32 0, i32 0
  %ptr.010 = load ptr, ptr %64, align 8
  %65 = getelementptr inbounds { ptr, i64 }, ptr %_46, i32 0, i32 1
  %ptr.111 = load i64, ptr %65, align 8
  %66 = getelementptr inbounds { ptr, i64 }, ptr %_6, i32 0, i32 0
  store ptr %ptr.010, ptr %66, align 8
  %67 = getelementptr inbounds { ptr, i64 }, ptr %_6, i32 0, i32 1
  store i64 %ptr.111, ptr %67, align 8
  %68 = getelementptr inbounds { ptr, i64 }, ptr %_6, i32 0, i32 0
  %69 = load ptr, ptr %68, align 8, !nonnull !1, !noundef !1
  %70 = getelementptr inbounds { ptr, i64 }, ptr %_6, i32 0, i32 1
  %71 = load i64, ptr %70, align 8
  %72 = getelementptr inbounds { ptr, i64 }, ptr %3, i32 0, i32 0
  store ptr %69, ptr %72, align 8
  %73 = getelementptr inbounds { ptr, i64 }, ptr %3, i32 0, i32 1
  store i64 %71, ptr %73, align 8
  br label %bb11
}

; alloc::slice::<impl alloc::borrow::ToOwned for [T]>::to_owned
; Function Attrs: uwtable
define void @"_ZN5alloc5slice64_$LT$impl$u20$alloc..borrow..ToOwned$u20$for$u20$$u5b$T$u5d$$GT$8to_owned17hb6043afda9e821aeE"(ptr sret(%"alloc::vec::Vec<u8>") %0, ptr align 1 %self.0, i64 %self.1) unnamed_addr #1 {
start:
; call <T as alloc::slice::hack::ConvertVec>::to_vec
  call void @"_ZN52_$LT$T$u20$as$u20$alloc..slice..hack..ConvertVec$GT$6to_vec17he4e409077b38c2b0E"(ptr sret(%"alloc::vec::Vec<u8>") %0, ptr align 1 %self.0, i64 %self.1)
  br label %bb1

bb1:                                              ; preds = %start
  ret void
}

; alloc::raw_vec::RawVec<T,A>::allocate_in
; Function Attrs: uwtable
define { ptr, i64 } @"_ZN5alloc7raw_vec19RawVec$LT$T$C$A$GT$11allocate_in17h29d1eec2f7a42a7bE"(i64 %capacity, i1 zeroext %0) unnamed_addr #1 personality ptr @__CxxFrameHandler3 {
start:
  %1 = alloca i64, align 8
  %_52 = alloca ptr, align 8
  %_37 = alloca i8, align 1
  %self = alloca ptr, align 8
  %_31 = alloca ptr, align 8
  %result = alloca { ptr, i64 }, align 8
  %_15 = alloca { i64, i64 }, align 8
  %_10 = alloca { i64, i64 }, align 8
  %_4 = alloca i8, align 1
  %2 = alloca { ptr, i64 }, align 8
  %alloc = alloca %"alloc::alloc::Global", align 1
  %init = alloca i8, align 1
  %3 = zext i1 %0 to i8
  store i8 %3, ptr %init, align 1
  store i8 1, ptr %_37, align 1
  br i1 false, label %bb1, label %bb2

bb1:                                              ; preds = %start
  store i8 1, ptr %_4, align 1
  br label %bb3

bb2:                                              ; preds = %start
  %_6 = icmp eq i64 %capacity, 0
  %4 = zext i1 %_6 to i8
  store i8 %4, ptr %_4, align 1
  br label %bb3

bb3:                                              ; preds = %bb1, %bb2
  %5 = load i8, ptr %_4, align 1, !range !4, !noundef !1
  %6 = trunc i8 %5 to i1
  br i1 %6, label %bb4, label %bb6

bb6:                                              ; preds = %bb3
  store i64 1, ptr %1, align 8
  %_39 = load i64, ptr %1, align 8, !range !5, !noundef !1
  br label %bb27

bb4:                                              ; preds = %bb3
  store i8 0, ptr %_37, align 1
; invoke alloc::raw_vec::RawVec<T,A>::new_in
  %7 = invoke { ptr, i64 } @"_ZN5alloc7raw_vec19RawVec$LT$T$C$A$GT$6new_in17h583530bb15b240f9E"()
          to label %bb5 unwind label %funclet_bb25

bb25:                                             ; preds = %funclet_bb25
  %8 = load i8, ptr %_37, align 1, !range !4, !noundef !1
  %9 = trunc i8 %8 to i1
  br i1 %9, label %bb24, label %bb23

funclet_bb25:                                     ; preds = %bb19, %bb15, %bb13, %bb10, %bb7, %bb27, %bb4
  %cleanuppad = cleanuppad within none []
  br label %bb25

bb5:                                              ; preds = %bb4
  store { ptr, i64 } %7, ptr %2, align 8
  br label %bb22

bb22:                                             ; preds = %bb21, %bb5
  %10 = getelementptr inbounds { ptr, i64 }, ptr %2, i32 0, i32 0
  %11 = load ptr, ptr %10, align 8, !nonnull !1, !noundef !1
  %12 = getelementptr inbounds { ptr, i64 }, ptr %2, i32 0, i32 1
  %13 = load i64, ptr %12, align 8
  %14 = insertvalue { ptr, i64 } undef, ptr %11, 0
  %15 = insertvalue { ptr, i64 } %14, i64 %13, 1
  ret { ptr, i64 } %15

bb27:                                             ; preds = %bb6
; invoke core::alloc::layout::Layout::array::inner
  %16 = invoke { i64, i64 } @_ZN4core5alloc6layout6Layout5array5inner17hd62e0f1362fcbab5E(i64 1, i64 %_39, i64 %capacity)
          to label %bb26 unwind label %funclet_bb25

bb26:                                             ; preds = %bb27
  store { i64, i64 } %16, ptr %_10, align 8
  %17 = getelementptr inbounds { i64, i64 }, ptr %_10, i32 0, i32 1
  %18 = load i64, ptr %17, align 8, !range !6, !noundef !1
  %19 = icmp eq i64 %18, 0
  %_12 = select i1 %19, i64 1, i64 0
  switch i64 %_12, label %bb8 [
    i64 0, label %bb9
    i64 1, label %bb7
  ]

bb8:                                              ; preds = %bb26
  unreachable

bb9:                                              ; preds = %bb26
  %20 = getelementptr inbounds { i64, i64 }, ptr %_10, i32 0, i32 0
  %layout.0 = load i64, ptr %20, align 8
  %21 = getelementptr inbounds { i64, i64 }, ptr %_10, i32 0, i32 1
  %layout.1 = load i64, ptr %21, align 8, !range !5, !noundef !1
  %22 = getelementptr inbounds { i64, i64 }, ptr %_15, i32 0, i32 1
  store i64 -9223372036854775807, ptr %22, align 8
  %23 = getelementptr inbounds { i64, i64 }, ptr %_15, i32 0, i32 1
  %24 = load i64, ptr %23, align 8, !range !7, !noundef !1
  %25 = sub i64 %24, -9223372036854775807
  %26 = icmp eq i64 %25, 0
  %_18 = select i1 %26, i64 0, i64 1
  switch i64 %_18, label %bb11 [
    i64 0, label %bb12
    i64 1, label %bb10
  ]

bb7:                                              ; preds = %bb26
; invoke alloc::raw_vec::capacity_overflow
  invoke void @_ZN5alloc7raw_vec17capacity_overflow17hda5aeeca668a5d1cE() #13
          to label %unreachable unwind label %funclet_bb25

unreachable:                                      ; preds = %bb19, %bb10, %bb7
  unreachable

bb11:                                             ; preds = %bb9
  unreachable

bb12:                                             ; preds = %bb9
  %27 = load i8, ptr %init, align 1, !range !4, !noundef !1
  %28 = trunc i8 %27 to i1
  %_21 = zext i1 %28 to i64
  switch i64 %_21, label %bb14 [
    i64 0, label %bb15
    i64 1, label %bb13
  ]

bb10:                                             ; preds = %bb9
; invoke alloc::raw_vec::capacity_overflow
  invoke void @_ZN5alloc7raw_vec17capacity_overflow17hda5aeeca668a5d1cE() #13
          to label %unreachable unwind label %funclet_bb25

bb14:                                             ; preds = %bb12
  unreachable

bb15:                                             ; preds = %bb12
; invoke <alloc::alloc::Global as core::alloc::Allocator>::allocate
  %29 = invoke { ptr, i64 } @"_ZN63_$LT$alloc..alloc..Global$u20$as$u20$core..alloc..Allocator$GT$8allocate17h33570ac3f03203afE"(ptr align 1 %alloc, i64 %layout.0, i64 %layout.1)
          to label %bb16 unwind label %funclet_bb25

bb13:                                             ; preds = %bb12
; invoke <alloc::alloc::Global as core::alloc::Allocator>::allocate_zeroed
  %30 = invoke { ptr, i64 } @"_ZN63_$LT$alloc..alloc..Global$u20$as$u20$core..alloc..Allocator$GT$15allocate_zeroed17h86c27fdccc866a56E"(ptr align 1 %alloc, i64 %layout.0, i64 %layout.1)
          to label %bb17 unwind label %funclet_bb25

bb17:                                             ; preds = %bb13
  store { ptr, i64 } %30, ptr %result, align 8
  br label %bb18

bb18:                                             ; preds = %bb16, %bb17
  %31 = load ptr, ptr %result, align 8
  %32 = icmp eq ptr %31, null
  %_27 = select i1 %32, i64 1, i64 0
  switch i64 %_27, label %bb20 [
    i64 0, label %bb21
    i64 1, label %bb19
  ]

bb16:                                             ; preds = %bb15
  store { ptr, i64 } %29, ptr %result, align 8
  br label %bb18

bb20:                                             ; preds = %bb18
  unreachable

bb21:                                             ; preds = %bb18
  %33 = getelementptr inbounds { ptr, i64 }, ptr %result, i32 0, i32 0
  %ptr.0 = load ptr, ptr %33, align 8, !nonnull !1, !noundef !1
  %34 = getelementptr inbounds { ptr, i64 }, ptr %result, i32 0, i32 1
  %ptr.1 = load i64, ptr %34, align 8
  store ptr %ptr.0, ptr %self, align 8
  %_51 = load ptr, ptr %self, align 8
  store ptr %_51, ptr %_52, align 8
  %35 = load ptr, ptr %_52, align 8, !nonnull !1, !noundef !1
  store ptr %35, ptr %_31, align 8
  %36 = load ptr, ptr %_31, align 8, !nonnull !1, !noundef !1
  store ptr %36, ptr %2, align 8
  %37 = getelementptr inbounds { ptr, i64 }, ptr %2, i32 0, i32 1
  store i64 %capacity, ptr %37, align 8
  br label %bb22

bb19:                                             ; preds = %bb18
; invoke alloc::alloc::handle_alloc_error
  invoke void @_ZN5alloc5alloc18handle_alloc_error17h41cb6a39a6ac85e8E(i64 %layout.0, i64 %layout.1) #13
          to label %unreachable unwind label %funclet_bb25

bb23:                                             ; preds = %bb24, %bb25
  cleanupret from %cleanuppad unwind to caller

bb24:                                             ; preds = %bb25
  br label %bb23
}

; alloc::raw_vec::RawVec<T,A>::current_memory
; Function Attrs: uwtable
define void @"_ZN5alloc7raw_vec19RawVec$LT$T$C$A$GT$14current_memory17h84f9d59a67bb0b1aE"(ptr sret(%"core::option::Option<(core::ptr::non_null::NonNull<u8>, core::alloc::layout::Layout)>") %0, ptr align 8 %self) unnamed_addr #1 {
start:
  %1 = alloca i64, align 8
  %pointer = alloca ptr, align 8
  %_11 = alloca ptr, align 8
  %_9 = alloca { ptr, { i64, i64 } }, align 8
  %_2 = alloca i8, align 1
  br i1 false, label %bb1, label %bb2

bb1:                                              ; preds = %start
  store i8 1, ptr %_2, align 1
  br label %bb3

bb2:                                              ; preds = %start
  %2 = getelementptr inbounds { ptr, i64 }, ptr %self, i32 0, i32 1
  %_5 = load i64, ptr %2, align 8
  %_4 = icmp eq i64 %_5, 0
  %3 = zext i1 %_4 to i8
  store i8 %3, ptr %_2, align 1
  br label %bb3

bb3:                                              ; preds = %bb1, %bb2
  %4 = load i8, ptr %_2, align 1, !range !4, !noundef !1
  %5 = trunc i8 %4 to i1
  br i1 %5, label %bb4, label %bb5

bb5:                                              ; preds = %bb3
  %6 = getelementptr inbounds { ptr, i64 }, ptr %self, i32 0, i32 1
  %n = load i64, ptr %6, align 8
  store i64 1, ptr %1, align 8
  %_15 = load i64, ptr %1, align 8, !range !5, !noundef !1
  br label %bb10

bb4:                                              ; preds = %bb3
  %7 = getelementptr inbounds %"core::option::Option<(core::ptr::non_null::NonNull<u8>, core::alloc::layout::Layout)>", ptr %0, i32 0, i32 1
  store i64 0, ptr %7, align 8
  br label %bb8

bb8:                                              ; preds = %bb7, %bb4
  ret void

bb10:                                             ; preds = %bb5
; call core::alloc::layout::Layout::array::inner
  %8 = call { i64, i64 } @_ZN4core5alloc6layout6Layout5array5inner17hd62e0f1362fcbab5E(i64 1, i64 %_15, i64 %n)
  %_7.0 = extractvalue { i64, i64 } %8, 0
  %_7.1 = extractvalue { i64, i64 } %8, 1
  br label %bb9

bb9:                                              ; preds = %bb10
; call core::result::Result<T,E>::unwrap_unchecked
  %9 = call { i64, i64 } @"_ZN4core6result19Result$LT$T$C$E$GT$16unwrap_unchecked17h8f0c2699931e05fdE"(i64 %_7.0, i64 %_7.1, ptr align 8 @alloc88)
  %layout.0 = extractvalue { i64, i64 } %9, 0
  %layout.1 = extractvalue { i64, i64 } %9, 1
  br label %bb6

bb6:                                              ; preds = %bb9
  %self1 = load ptr, ptr %self, align 8, !nonnull !1, !noundef !1
  store ptr %self1, ptr %pointer, align 8
  %_29 = load ptr, ptr %pointer, align 8, !nonnull !1, !noundef !1
  store ptr %_29, ptr %_11, align 8
  %10 = load ptr, ptr %_11, align 8, !nonnull !1, !noundef !1
; call <T as core::convert::Into<U>>::into
  %_10 = call ptr @"_ZN50_$LT$T$u20$as$u20$core..convert..Into$LT$U$GT$$GT$4into17h4121e13b91be4aecE"(ptr %10)
  br label %bb7

bb7:                                              ; preds = %bb6
  store ptr %_10, ptr %_9, align 8
  %11 = getelementptr inbounds { ptr, { i64, i64 } }, ptr %_9, i32 0, i32 1
  %12 = getelementptr inbounds { i64, i64 }, ptr %11, i32 0, i32 0
  store i64 %layout.0, ptr %12, align 8
  %13 = getelementptr inbounds { i64, i64 }, ptr %11, i32 0, i32 1
  store i64 %layout.1, ptr %13, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %0, ptr align 8 %_9, i64 24, i1 false)
  br label %bb8
}

; alloc::raw_vec::RawVec<T,A>::new_in
; Function Attrs: uwtable
define { ptr, i64 } @"_ZN5alloc7raw_vec19RawVec$LT$T$C$A$GT$6new_in17h583530bb15b240f9E"() unnamed_addr #1 personality ptr @__CxxFrameHandler3 {
start:
  %0 = alloca ptr, align 8
  %pointer = alloca ptr, align 8
  %_2 = alloca ptr, align 8
  %1 = alloca { ptr, i64 }, align 8
  store i64 1, ptr %0, align 8
  %ptr = load ptr, ptr %0, align 8
  br label %bb3

bb3:                                              ; preds = %start
  store ptr %ptr, ptr %pointer, align 8
  %_12 = load ptr, ptr %pointer, align 8, !nonnull !1, !noundef !1
  store ptr %_12, ptr %_2, align 8
  %2 = load ptr, ptr %_2, align 8, !nonnull !1, !noundef !1
  store ptr %2, ptr %1, align 8
  %3 = getelementptr inbounds { ptr, i64 }, ptr %1, i32 0, i32 1
  store i64 0, ptr %3, align 8
  %4 = getelementptr inbounds { ptr, i64 }, ptr %1, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8, !nonnull !1, !noundef !1
  %6 = getelementptr inbounds { ptr, i64 }, ptr %1, i32 0, i32 1
  %7 = load i64, ptr %6, align 8
  %8 = insertvalue { ptr, i64 } undef, ptr %5, 0
  %9 = insertvalue { ptr, i64 } %8, i64 %7, 1
  ret { ptr, i64 } %9

bb1:                                              ; preds = %funclet_bb1
  br label %bb2

bb2:                                              ; preds = %bb1
  cleanupret from %cleanuppad unwind to caller

funclet_bb1:                                      ; No predecessors!
  %cleanuppad = cleanuppad within none []
  br label %bb1
}

; <alloc::alloc::Global as core::alloc::Allocator>::deallocate
; Function Attrs: inlinehint uwtable
define internal void @"_ZN63_$LT$alloc..alloc..Global$u20$as$u20$core..alloc..Allocator$GT$10deallocate17h1bf610692cc908ffE"(ptr align 1 %self, ptr %ptr, i64 %0, i64 %1) unnamed_addr #0 {
start:
  %self2 = alloca i64, align 8
  %layout1 = alloca { i64, i64 }, align 8
  %layout = alloca { i64, i64 }, align 8
  %2 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 0
  store i64 %0, ptr %2, align 8
  %3 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 1
  store i64 %1, ptr %3, align 8
  %_4 = load i64, ptr %layout, align 8
  %4 = icmp eq i64 %_4, 0
  br i1 %4, label %bb2, label %bb1

bb2:                                              ; preds = %start
  br label %bb3

bb1:                                              ; preds = %start
  %5 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 0
  %6 = load i64, ptr %5, align 8
  %7 = getelementptr inbounds { i64, i64 }, ptr %layout, i32 0, i32 1
  %8 = load i64, ptr %7, align 8, !range !5, !noundef !1
  %9 = getelementptr inbounds { i64, i64 }, ptr %layout1, i32 0, i32 0
  store i64 %6, ptr %9, align 8
  %10 = getelementptr inbounds { i64, i64 }, ptr %layout1, i32 0, i32 1
  store i64 %8, ptr %10, align 8
  %_11 = load i64, ptr %layout1, align 8
  %11 = getelementptr inbounds { i64, i64 }, ptr %layout1, i32 0, i32 1
  %12 = load i64, ptr %11, align 8, !range !5, !noundef !1
  store i64 %12, ptr %self2, align 8
  %_16 = load i64, ptr %self2, align 8, !range !5, !noundef !1
  call void @__rust_dealloc(ptr %ptr, i64 %_11, i64 %_16) #15
  br label %bb4

bb4:                                              ; preds = %bb1
  br label %bb3

bb3:                                              ; preds = %bb2, %bb4
  ret void
}

; <alloc::alloc::Global as core::alloc::Allocator>::allocate_zeroed
; Function Attrs: inlinehint uwtable
define internal { ptr, i64 } @"_ZN63_$LT$alloc..alloc..Global$u20$as$u20$core..alloc..Allocator$GT$15allocate_zeroed17h86c27fdccc866a56E"(ptr align 1 %self, i64 %layout.0, i64 %layout.1) unnamed_addr #0 {
start:
; call alloc::alloc::Global::alloc_impl
  %0 = call { ptr, i64 } @_ZN5alloc5alloc6Global10alloc_impl17h3e29304841a7fef4E(ptr align 1 %self, i64 %layout.0, i64 %layout.1, i1 zeroext true)
  %1 = extractvalue { ptr, i64 } %0, 0
  %2 = extractvalue { ptr, i64 } %0, 1
  br label %bb1

bb1:                                              ; preds = %start
  %3 = insertvalue { ptr, i64 } undef, ptr %1, 0
  %4 = insertvalue { ptr, i64 } %3, i64 %2, 1
  ret { ptr, i64 } %4
}

; <alloc::alloc::Global as core::alloc::Allocator>::allocate
; Function Attrs: inlinehint uwtable
define internal { ptr, i64 } @"_ZN63_$LT$alloc..alloc..Global$u20$as$u20$core..alloc..Allocator$GT$8allocate17h33570ac3f03203afE"(ptr align 1 %self, i64 %layout.0, i64 %layout.1) unnamed_addr #0 {
start:
; call alloc::alloc::Global::alloc_impl
  %0 = call { ptr, i64 } @_ZN5alloc5alloc6Global10alloc_impl17h3e29304841a7fef4E(ptr align 1 %self, i64 %layout.0, i64 %layout.1, i1 zeroext false)
  %1 = extractvalue { ptr, i64 } %0, 0
  %2 = extractvalue { ptr, i64 } %0, 1
  br label %bb1

bb1:                                              ; preds = %start
  %3 = insertvalue { ptr, i64 } undef, ptr %1, 0
  %4 = insertvalue { ptr, i64 } %3, i64 %2, 1
  ret { ptr, i64 } %4
}

; <alloc::vec::Vec<T,A> as core::ops::drop::Drop>::drop
; Function Attrs: uwtable
define void @"_ZN70_$LT$alloc..vec..Vec$LT$T$C$A$GT$$u20$as$u20$core..ops..drop..Drop$GT$4drop17hdcde2af413610940E"(ptr align 8 %self) unnamed_addr #1 {
start:
  %_11 = alloca { ptr, i64 }, align 8
  %_10 = alloca %"core::ptr::metadata::PtrRepr<[u8]>", align 8
; call alloc::vec::Vec<T,A>::as_mut_ptr
  %data = call ptr @"_ZN5alloc3vec16Vec$LT$T$C$A$GT$10as_mut_ptr17h546501678f6b91ebE"(ptr align 8 %self)
  br label %bb1

bb1:                                              ; preds = %start
  %0 = getelementptr inbounds %"alloc::vec::Vec<u8>", ptr %self, i32 0, i32 1
  %len = load i64, ptr %0, align 8
  store ptr %data, ptr %_11, align 8
  %1 = getelementptr inbounds { ptr, i64 }, ptr %_11, i32 0, i32 1
  store i64 %len, ptr %1, align 8
  %2 = getelementptr inbounds { ptr, i64 }, ptr %_11, i32 0, i32 0
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds { ptr, i64 }, ptr %_11, i32 0, i32 1
  %5 = load i64, ptr %4, align 8
  %6 = getelementptr inbounds { ptr, i64 }, ptr %_10, i32 0, i32 0
  store ptr %3, ptr %6, align 8
  %7 = getelementptr inbounds { ptr, i64 }, ptr %_10, i32 0, i32 1
  store i64 %5, ptr %7, align 8
  %8 = getelementptr inbounds { ptr, i64 }, ptr %_10, i32 0, i32 0
  %_2.0 = load ptr, ptr %8, align 8
  %9 = getelementptr inbounds { ptr, i64 }, ptr %_10, i32 0, i32 1
  %_2.1 = load i64, ptr %9, align 8
  br label %bb2

bb2:                                              ; preds = %bb1
  ret void
}

; <alloc::raw_vec::RawVec<T,A> as core::ops::drop::Drop>::drop
; Function Attrs: uwtable
define void @"_ZN77_$LT$alloc..raw_vec..RawVec$LT$T$C$A$GT$$u20$as$u20$core..ops..drop..Drop$GT$4drop17h20d6bc0da571b7f6E"(ptr align 8 %self) unnamed_addr #1 {
start:
  %_2 = alloca %"core::option::Option<(core::ptr::non_null::NonNull<u8>, core::alloc::layout::Layout)>", align 8
; call alloc::raw_vec::RawVec<T,A>::current_memory
  call void @"_ZN5alloc7raw_vec19RawVec$LT$T$C$A$GT$14current_memory17h84f9d59a67bb0b1aE"(ptr sret(%"core::option::Option<(core::ptr::non_null::NonNull<u8>, core::alloc::layout::Layout)>") %_2, ptr align 8 %self)
  br label %bb1

bb1:                                              ; preds = %start
  %0 = getelementptr inbounds %"core::option::Option<(core::ptr::non_null::NonNull<u8>, core::alloc::layout::Layout)>", ptr %_2, i32 0, i32 1
  %1 = load i64, ptr %0, align 8, !range !6, !noundef !1
  %2 = icmp eq i64 %1, 0
  %_4 = select i1 %2, i64 0, i64 1
  %3 = icmp eq i64 %_4, 1
  br i1 %3, label %bb2, label %bb4

bb2:                                              ; preds = %bb1
  %ptr = load ptr, ptr %_2, align 8, !nonnull !1, !noundef !1
  %4 = getelementptr inbounds { ptr, { i64, i64 } }, ptr %_2, i32 0, i32 1
  %5 = getelementptr inbounds { i64, i64 }, ptr %4, i32 0, i32 0
  %layout.0 = load i64, ptr %5, align 8
  %6 = getelementptr inbounds { i64, i64 }, ptr %4, i32 0, i32 1
  %layout.1 = load i64, ptr %6, align 8, !range !5, !noundef !1
; call <alloc::alloc::Global as core::alloc::Allocator>::deallocate
  call void @"_ZN63_$LT$alloc..alloc..Global$u20$as$u20$core..alloc..Allocator$GT$10deallocate17h1bf610692cc908ffE"(ptr align 1 %self, ptr %ptr, i64 %layout.0, i64 %layout.1)
  br label %bb3

bb4:                                              ; preds = %bb3, %bb1
  ret void

bb3:                                              ; preds = %bb2
  br label %bb4
}

; probe1::probe
; Function Attrs: uwtable
define void @_ZN6probe15probe17h428f7637126b16c3E() unnamed_addr #1 {
start:
  %_10 = alloca [1 x { ptr, ptr }], align 8
  %_3 = alloca %"core::fmt::Arguments", align 8
  %res = alloca %"alloc::string::String", align 8
  %_1 = alloca %"alloc::string::String", align 8
; call core::fmt::ArgumentV1::new_lower_exp
  %0 = call { ptr, ptr } @_ZN4core3fmt10ArgumentV113new_lower_exp17h31ae6fce06b32472E(ptr align 8 @alloc6)
  %_11.0 = extractvalue { ptr, ptr } %0, 0
  %_11.1 = extractvalue { ptr, ptr } %0, 1
  br label %bb1

bb1:                                              ; preds = %start
  %1 = getelementptr inbounds [1 x { ptr, ptr }], ptr %_10, i64 0, i64 0
  %2 = getelementptr inbounds { ptr, ptr }, ptr %1, i32 0, i32 0
  store ptr %_11.0, ptr %2, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %1, i32 0, i32 1
  store ptr %_11.1, ptr %3, align 8
; call core::fmt::Arguments::new_v1
  call void @_ZN4core3fmt9Arguments6new_v117ha40a2220dbd1c6f3E(ptr sret(%"core::fmt::Arguments") %_3, ptr align 8 @alloc4, i64 1, ptr align 8 %_10, i64 1)
  br label %bb2

bb2:                                              ; preds = %bb1
; call alloc::fmt::format
  call void @_ZN5alloc3fmt6format17hdd81d5776ef6c6b2E(ptr sret(%"alloc::string::String") %res, ptr %_3)
  br label %bb3

bb3:                                              ; preds = %bb2
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %_1, ptr align 8 %res, i64 24, i1 false)
; call core::ptr::drop_in_place<alloc::string::String>
  call void @"_ZN4core3ptr42drop_in_place$LT$alloc..string..String$GT$17h1ae547631abb8825E"(ptr %_1)
  br label %bb4

bb4:                                              ; preds = %bb3
  ret void
}

; core::fmt::num::imp::<impl core::fmt::LowerExp for isize>::fmt
; Function Attrs: uwtable
declare zeroext i1 @"_ZN4core3fmt3num3imp55_$LT$impl$u20$core..fmt..LowerExp$u20$for$u20$isize$GT$3fmt17h05ae56db2110b6c6E"(ptr align 8, ptr align 8) unnamed_addr #1

; core::panicking::panic_fmt
; Function Attrs: cold noinline noreturn uwtable
declare void @_ZN4core9panicking9panic_fmt17hdcb6d460fcce0538E(ptr, ptr align 8) unnamed_addr #3

declare i32 @__CxxFrameHandler3(...) unnamed_addr #4

; Function Attrs: argmemonly nocallback nofree nounwind willreturn
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #5

; Function Attrs: nocallback nofree nosync nounwind readnone willreturn
declare i1 @llvm.expect.i1(i1, i1) #6

; core::panicking::panic
; Function Attrs: cold noinline noreturn uwtable
declare void @_ZN4core9panicking5panic17h461763e65d8957b4E(ptr align 1, i64, ptr align 8) unnamed_addr #3

; alloc::fmt::format::format_inner
; Function Attrs: uwtable
declare void @_ZN5alloc3fmt6format12format_inner17hcaab02f56890c3e4E(ptr sret(%"alloc::string::String"), ptr) unnamed_addr #1

; Function Attrs: inaccessiblememonly nocallback nofree nosync nounwind willreturn
declare void @llvm.assume(i1 noundef) #7

; Function Attrs: nounwind allockind("alloc,zeroed,aligned") allocsize(0) uwtable
declare noalias ptr @__rust_alloc_zeroed(i64, i64 allocalign) unnamed_addr #8

; Function Attrs: nounwind allockind("alloc,uninitialized,aligned") allocsize(0) uwtable
declare noalias ptr @__rust_alloc(i64, i64 allocalign) unnamed_addr #9

; alloc::raw_vec::capacity_overflow
; Function Attrs: noreturn uwtable
declare void @_ZN5alloc7raw_vec17capacity_overflow17hda5aeeca668a5d1cE() unnamed_addr #10

; alloc::alloc::handle_alloc_error
; Function Attrs: cold noreturn uwtable
declare void @_ZN5alloc5alloc18handle_alloc_error17h41cb6a39a6ac85e8E(i64, i64) unnamed_addr #11

; Function Attrs: nounwind allockind("free") uwtable
declare void @__rust_dealloc(ptr allocptr, i64, i64) unnamed_addr #12

attributes #0 = { inlinehint uwtable "target-cpu"="x86-64" }
attributes #1 = { uwtable "target-cpu"="x86-64" }
attributes #2 = { inlinehint noreturn uwtable "target-cpu"="x86-64" }
attributes #3 = { cold noinline noreturn uwtable "target-cpu"="x86-64" }
attributes #4 = { "target-cpu"="x86-64" }
attributes #5 = { argmemonly nocallback nofree nounwind willreturn }
attributes #6 = { nocallback nofree nosync nounwind readnone willreturn }
attributes #7 = { inaccessiblememonly nocallback nofree nosync nounwind willreturn }
attributes #8 = { nounwind allockind("alloc,zeroed,aligned") allocsize(0) uwtable "alloc-family"="__rust_alloc" "target-cpu"="x86-64" }
attributes #9 = { nounwind allockind("alloc,uninitialized,aligned") allocsize(0) uwtable "alloc-family"="__rust_alloc" "target-cpu"="x86-64" }
attributes #10 = { noreturn uwtable "target-cpu"="x86-64" }
attributes #11 = { cold noreturn uwtable "target-cpu"="x86-64" }
attributes #12 = { nounwind allockind("free") uwtable "alloc-family"="__rust_alloc" "target-cpu"="x86-64" }
attributes #13 = { noreturn }
attributes #14 = { noinline }
attributes #15 = { nounwind }

!llvm.module.flags = !{!0}

!0 = !{i32 7, !"PIC Level", i32 2}
!1 = !{}
!2 = !{i64 1}
!3 = !{i64 8}
!4 = !{i8 0, i8 2}
!5 = !{i64 1, i64 -9223372036854775807}
!6 = !{i64 0, i64 -9223372036854775807}
!7 = !{i64 0, i64 -9223372036854775806}
