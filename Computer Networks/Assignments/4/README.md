Assignment 4: Jumping Bozons (40 marks)
Due: Friday, April 8, 2022 (4:00pm)
Learning Objectives
The purpose of this assignment is to learn about Medium Access Control (MAC) protocols. Along the way, you will also learn how to develop and use a simple discrete-event simulation for the performance analysis of network protocols.

Background Scenario
The "Planet of the Jumping Bozons" is a wonderful place, and you are delighted that the Bozons have invited you there on an all-expenses-paid trip as a network protocol performance consultant.

The Bozons are curious but friendly creatures who spend all of their lives alternating between two states: sleeping and yodeling. Other needs, such as nourishment and waste elimination, occur magically during the sleeping state, by osmosis with the planet's nutrient-rich atmosphere.

The durations of the sleep periods are independent and identically distributed (iid) random variables drawn from an exponential distribution with a mean of S time units (e.g., S = 8.5 hours). The durations of the yodeling periods are iid random variables drawn from an exponential distribution with a mean of Y time units (e.g., Y = 1.2 hours).

Bozons live in colonies of size M. In such a colony, there are M beds laid out in a perfectly straight line, with all of the beds exactly one meter apart. Each Bozon has a designated bed, which is used as the home base for all of its activities. When sleeping, the Bozon is horizontal on the bed and silent. When it is time to yodel, the Bozon awakes instantly, leaps high into the air, and emits a melodious yodel (song). Once the (randomly chosen) duration of the yodeling period has elapsed, the Bozon lands gently back on the bed and instantly falls asleep.

Bozons are so fascinating that busloads of tourists come from miles around just to watch them. In fact, tourism is the main source of revenue for the Bozon planet.

The complication that arises is the following: if more than one Bozon yodels at the same time, the result is an ear-piercing screech that drives all tourists away This screechy sound is in stark contrast to the melodious sound savoured by the tourists when a single Bozon is yodeling, and the peaceful night air when all Bozons are sleeping. To further complicate matters, the planet is shrouded in eternal darkness (thus the Bozons cannot see which other Bozons are awake) and all Bozons are tone-deaf (and thus cannot hear each other yodeling).

The leader of the Bozons has proposed building larger and larger colonies to really draw in the tourists. Other Bozons are concerned about the scalability of the random channel access protocol, and that is why they have brought you here. They want your recommendations in this regard.

Your Tasks
Your task in this assignment is to study the performance of the Bozon channel access protocol. Rather than staying up all night observing and recording the yodeling and screeching in different Bozon colonies, you will write a simple computer simulation program in C or C++ to study this problem.

Specifically, write a computer simulation program (in C or C++) that can model (i.e., simulate) the channel access protocol used in a Bozon colony, and determine the average channel performance. For assessing "channel performance", you should identify the (average) percentage of the time that the channel is silent (i.e., not very interesting to tourists), melodious (i.e., very interesting to tourists), and screechy (i.e., intolerable to tourists). Your program should also report the number of attempted yodels, and the percentage of attempted yodels that were perfect. A "perfect yodel" is one that was melodious from start to finish, without any interruption by other Bozons. (Hint: Tracking this information will be very helpful for debugging your program.)

Your program should be easily parameterizable for different values of M, S, and Y (either on the command line, or by prompting the user, or as manifest constants in the source code). You will use these parameters to explore several Bozon configuration scenarios listed below. You may assume that all the Bozons are initially asleep at time zero.

When you are finished, please submit your solution in electronic form to your TA via D2L. Your submission should include the source code for your Bozon protocol simulator, a very brief user manual, and your simulation results (in graphical and/or tabular form).

Grading Scheme
The proposed grading scheme for the assignment is as follows:

20 marks for the design and implementation of a credible simulator for the Bozon channel access protocol
4 marks for a brief user manual describing how to compile and use your simulator, and a brief description of your testing
4 marks for simulation results evaluating the channel performance for a small Bozon colony, where M = 5, S = 100.0 time units, and Y = 10.0 time units. Show the average percentages achieved for each of the performance metrics stated above, from a "long enough" observation period to have some statistical confidence in your result (e.g., 1,000,000.0 simulated time units).
4 marks for simulation results to determine the optimal colony size M when S = 100.0 and Y = 10.0. That is, use your program to determine the maximum "melodious" percentage that can be achieved, as a function of M. State this percentage value, as well as the value(s) of M at which it is achieved.
4 marks for simulation experiments to assess the scalability of Bozon colonies when S = 200.0 time units, and Y = 10.0 time units. Produce a graph or table (or both) that clearly shows the percentages achieved for each of the performance metrics above, as a function of M, using M = 1, 2, 4, 8, 16, 32, 64, and 128. Make sure to indicate the highest "melodious" value achieved, and the value of M at which it occurs.
4 marks for a short summary (at most 1 page) of your results and observations about the Bozon protocol
There is NO DEMO required for this assignment.

Bonus (optional)
The Bozon parliament is considering the establishment of a standardized "national yodel" of fixed time duration 10.0 time units. If each Bozon emitted this standardized yodel during their wake period, rather than a random (i.e., exponentially distributed) duration, would the channel performance be better, worse, or the same? Use your simulator to explore this scenario. Show your results (2 marks), and explain them as best you can (2 marks).

Tips
When debugging your code, use a small network topology (e.g., M = 4, or smaller). When you are sure your results make sense, scale up the network size and do your final runs.
One way to generate exponentially-distributed random numbers in C is indicated here. Note that exponential random variables are continuous (i.e., floating point) values, and not discrete (i.e., integer) values. Please do not round these values off to the nearest integer.
