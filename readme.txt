Hua Yang, 128-00-2637
Erik Kamp, 132-00-4838
Sharlina Keshava, 140-00-9007

Problems that we encountered:
* We were unable to fprintf beyond 8 lines of text into the buffer at a time.
* This is only an issue when running /factor and /fib, where the output can be more than 8 lines.