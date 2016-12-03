### Hello World overload testing



***First test***

https://overload.yandex.net/3283

Firstly I ran a test with a line rising load starting from 1000 requests/sec (rps) to 5000 rps during a time period of 2 minutes. The test showed that first problems began at ~2667 rps (http-502 codes ==  Bad Gateway). If we look at Monitoring tab we can see that after 1900 rps CPU use is close to 75% and after 2400 rps it comes to 100%. At the same time we can notice some peaks in disk graph (writing) at 2133 and 2967 rps, these problems is thought to be caused by some system processes that occured during the test.

Results: load higher than 2000 rps can be unsafe. So we can specify the upper load bound as 2000 rps. Now we need to test how our service will work at that load if it remains the same during few minutes.


***Second  test***

https://overload.yandex.net/3284

Second test started from a 100 rps load rising it to 2000 rps in 30 seconds. Then the load stayed constant during 150 seconds. On http-codes graph we can mention a number of peaks with 502 http code. They are not really long or frequent but it's still a problem cause these peaks means few hundreds Bad Gateway responses. CPU use throughout the whole graph is around 75% with some peaks to 100%. Disk writing peaks also remained. Actually that doesn't conflict with the theory of operating system writing writing calls.

Results: it would be nice to reduce a quantity of 502-http codes. Let's try one more test with a lower upper load bound.

***Third  test***

https://overload.yandex.net/3285

Third test started from a 100 rps load rising it to 1800 rps in 30 seconds. Then the load stayed constant during 150 seconds. Now we get less http 502 codes but the quantity of it still differs from 0. CPU use is under 75% almost everywhere and disk writing peaks still happen.

Results: it's thought to be needed to reduce load a little more.

***Fourth  test***

https://overload.yandex.net/3440

Fourth test started from a 100 rps load rising it to 1500 rps in 30 seconds. Then the load stayed constant during 150 seconds. We got all the http codes equal to http_200 (OK). The situation with CPU and disk is also acceptable.  

Results: our HelloWorld web service works fine at a load of ~1500 rps.

Cumulative response time quantiles:

85% < 1 ms

95% < 3 ms

99% < 20 ms
