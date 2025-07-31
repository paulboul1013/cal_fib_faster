# cal_fib_faster
calcualte fibonacci number with gmp and multi-thread

# install gmp lib
```clike
apt-get install  libgmp3-dev
```

# Fibonacci 數列與快速倍增法
快速倍增法使用以下數學公式：

$F(2k)   = F(k) × [2×F(k+1) − F(k)]$  
$F(2k+1) = F(k+1)^2 + F(k)^2$

# 變數定義
`mpz_t a, b`：代表目前的 Fibonacci 組 (F(n), F(n+1))

`mpz_t p, q`：這是倍增過程中的遞推變數，初始為 (0, 1)；即 F(0), F(1)

`tmp`：暫存變數，避免中間重複計算。

`count`：要算的 Fibonacci 項數，會逐步除以 2 或減 1。

