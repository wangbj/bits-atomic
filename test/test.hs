{-# LANGUAGE ForeignFunctionInterface, RankNTypes, FlexibleContexts #-}
-- Test framework imports
import Test.Framework (defaultMain, testGroup, Test)
import Test.Framework.Providers.HUnit
import Test.HUnit hiding (Test, Testable)
import Test.Framework.Providers.QuickCheck2
import Test.QuickCheck

-- Code to test
import Data.Bits.Atomic
import Foreign.Marshal.Alloc
import Foreign.Storable
import Data.Word
import Data.Int
import Data.Bits

tests :: [Test]
tests = [ testGroup "Test Cases" 
          [ testCase "compareAndSwap" test_compareAndSwap_all
          , testCase "compareAndSwapBool" test_compareAndSwapBool_all
          , testCase "lockTestAndSet and lockRelease" test_lockTestRelease_all
          , testCase "fetchAndAdd" test_fetchAndAdd_all
          , testCase "fetchAndSub" test_fetchAndSub_all
          , testCase "fetchAndXor" test_fetchAndXor_all
          , testCase "subAndFetch" test_subAndFetch_all
          ]
        ]

main :: IO ()
main = defaultMain tests

-- TODO: Use Quickcheck to produce arbitrary values?
testPattern :: Integral a => a
testPattern = 0xdeadbeef

type PolyTest = forall a. (AtomicBits a, Storable a, Integral a, Bounded a, Show a) => a -> Assertion

testTypes :: PolyTest -> Assertion
testTypes _test = do
    _test (testPattern :: Int)
    _test (testPattern :: Word)
    _test (testPattern :: Int8)
    _test (testPattern :: Int16)
    _test (testPattern :: Int32)
    _test (testPattern :: Int64)
    _test (testPattern :: Word8)
    _test (testPattern :: Word16)
    _test (testPattern :: Word32)
    _test (testPattern :: Word64)

test_compareAndSwap :: (AtomicBits a, Storable a, Integral a, Bounded a, Show a) =>
             a -> Assertion
test_compareAndSwap i = alloca $ \p -> do
    poke p i
    old1 <- compareAndSwap p i 2
    old1 @?= i
    old2 <- compareAndSwap p 3 3
    old2 @?= 2
    new <- peek p
    new @?= 2
    old3 <- compareAndSwap p 2 3
    old3 @?= 2
test_compareAndSwap_all :: Assertion
test_compareAndSwap_all = testTypes test_compareAndSwap

test_compareAndSwapBool :: (AtomicBits a, Storable a, Integral a, Bounded a, Show a) =>
             a -> Assertion
test_compareAndSwapBool i = alloca $ \p -> do
    poke p i
    swap0 <- compareAndSwapBool p i 2
    swap0 @?= True
    swap1 <- compareAndSwapBool p 3 3
    swap1 @?= False
    new <- peek p
    new @?= 2
    swap2 <- compareAndSwapBool p 2 3
    swap2 @?= True
test_compareAndSwapBool_all :: Assertion
test_compareAndSwapBool_all = testTypes test_compareAndSwapBool

test_fetchAndAdd :: (AtomicBits a, Storable a, Integral a, Bounded a, Show a) =>
                 a -> Assertion
test_fetchAndAdd i = alloca $ \p -> do
    poke p i
    old0 <- fetchAndAdd p 1
    old0 @?= i
    new <- peek p
    new @?= i + 1
    poke p (maxBound `asTypeOf` i)
    old1 <- fetchAndAdd p 1
    old1 @?= (maxBound  `asTypeOf` i)
    new1 <- peek p
    new1 @?= (minBound `asTypeOf` i)

test_fetchAndAdd_all :: Assertion
test_fetchAndAdd_all = testTypes test_fetchAndAdd


test_fetchAndSub :: (AtomicBits a, Storable a, Integral a, Bounded a, Show a) =>
                 a -> Assertion
test_fetchAndSub i = alloca $ \p -> do
    poke p i
    old0 <- fetchAndSub p 1
    old0 @?= i
    new <- peek p
    new @?= i - 1
    poke p (minBound `asTypeOf` i)
    old1 <- fetchAndSub p 1
    old1 @?= (minBound  `asTypeOf` i)
    new1 <- peek p
    new1 @?= (maxBound `asTypeOf` i)
test_fetchAndSub_all :: Assertion
test_fetchAndSub_all = testTypes test_fetchAndSub


test_fetchAndXor :: (AtomicBits a, Storable a, Integral a, Bounded a, Show a) =>
                 a -> Assertion
test_fetchAndXor i = alloca $ \p -> do
    poke p i
    old0 <- fetchAndXor p 1
    old0 @?= i
    new <- peek p
    new @?= i `xor` 1
test_fetchAndXor_all :: Assertion
test_fetchAndXor_all = testTypes test_fetchAndXor


test_subAndFetch :: (AtomicBits a, Storable a, Integral a, Bounded a, Show a) =>
                 a -> Assertion
test_subAndFetch i = alloca $ \p -> do
    poke p i
    new0 <- subAndFetch p 1
    new0 @?= i - 1
    poke p (minBound `asTypeOf` i)
    new1 <- subAndFetch p 1
    new1 @?= (maxBound  `asTypeOf` i)

test_subAndFetch_all :: Assertion
test_subAndFetch_all = testTypes test_subAndFetch

test_lockTestRelease :: (AtomicBits a, Storable a, Integral a, Bounded a, Show a) =>
                 a -> Assertion
test_lockTestRelease i = alloca $ \p -> do
    poke p i
    new0 <- lockTestAndSet p
    new0 @?= i
    new1 <- peek p
    new1 @?= 1
    lockRelease p
    new2 <- peek p
    new2 @?= 0
test_lockTestRelease_all :: Assertion
test_lockTestRelease_all = testTypes test_lockTestRelease
