## Introduction

This is a terminal application written in C, that allows the user to do different operations on texts related to the Caesar cipher. These operations are:

- read text from file or terminal
- write to terminal
- shift the text for encryption or decryption
- display the histogram of a text
- break the Caesar cipher with frequency analysis, being able to choose between three distance functions:
    - Chi-squared distance
    - Euclidean distance
    - Cosine distance

What follows is an analysis of the three distance functions and the frequency analysis method.

## Analysis of the distance functions

To be able to analyze how well each distance function and the frequency analysis as a whole works, 17 texts were generated using Mistral AI's [Le chat](https://chat.mistral.ai/chat). The texts and the top three shifts of each distance function are noted in the table below.

| Nr  | Text   | Encryption shift       | Chi-squared 1                     | Chi-squared 2 | Chi-squared 3 | Euclidean 1 | Euclidean 2 | Euclidean 3 | Cosine 1 | Cosine 2 | Cosine 3 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |:---: | :---: | :---: |
|1|Hello!|11|Shift = 22, Dist = 399.07|Shift = 11, Dist = 450.70|Shift = 14, Dist = 527.03|Shift = 18, Dist = 43.13|Shift = 11, Dist = 45.66|Shift = 14, Dist = 46.57|Shift = 18, Dist = 0.41|Shift = 11, Dist = 0.49|Shift = 14, Dist = 0.53|
|2|Hello, World!|17|Shift = 20, Dist = 239.93|Shift = 2, Dist = 246.61|Shift = 17, Dist = 284.49|Shift = 20, Dist = 33.11|Shift = 24, Dist = 34.36|Shift = 17, Dist = 35.87|Shift = 20, Dist = 0.37|Shift = 24, Dist = 0.41|Shift = 17, Dist = 0.46|
|3|Hello, World! Hello, World! Hello, World! Hello, World!|5|Shift = 8, Dist = 239.93|Shift = 16, Dist = 246.61|Shift = 5, Dist = 284.49|Shift = 8, Dist = 33.11|Shift = 12, Dist = 34.36|Shift = 5, Dist = 35.87|Shift = 8, Dist = 0.37|Shift = 12, Dist = 0.41|Shift = 5, Dist = 0.46|
|4|How's it going?|10|Shift = 10, Dist = 239.41|Shift = 11, Dist = 339.39|Shift = 22, Dist = 570.09|Shift = 24, Dist = 29.44|Shift = 10, Dist = 30.08|Shift = 25, Dist = 30.62|Shift = 24, Dist = 0.38|Shift = 10, Dist = 0.40|Shift = 25, Dist = 0.42|
|5|Hello, world! How's it going?|16|Shift = 16, Dist = 130.98|Shift = 19, Dist = 387.53|Shift = 12, Dist = 406.45|Shift = 16, Dist = 23.65|Shift = 19, Dist = 25.31|Shift = 4, Dist = 26.86|Shift = 16, Dist = 0.30|Shift = 19, Dist = 0.35|Shift = 4, Dist = 0.40|
|6|Meet me later|24|Shift = 24, Dist = 223.00|Shift = 2, Dist = 343.09|Shift = 17, Dist = 375.51|Shift = 24, Dist = 34.86|Shift = 2, Dist = 41.13|Shift = 9, Dist = 41.19|Shift = 24, Dist = 0.31|Shift = 2, Dist = 0.51|Shift = 9, Dist = 0.51|
|7|Durian, elderberry, fig|20|Shift = 20, Dist = 118.49|Shift = 11, Dist = 280.02|Shift = 18, Dist = 321.00|Shift = 7, Dist = 22.80|Shift = 20, Dist = 24.31|Shift = 11, Dist = 28.02|Shift = 7, Dist = 0.26|Shift = 20, Dist = 0.30|Shift = 11, Dist = 0.42|
|8|Mississippi river cruise|16|Shift = 16, Dist = 213.57|Shift = 20, Dist = 309.57|Shift = 5, Dist = 348.26|Shift = 20, Dist = 27.84|Shift = 16, Dist = 34.23|Shift = 5, Dist = 36.03|Shift = 20, Dist = 0.25|Shift = 16, Dist = 0.44|Shift = 5, Dist = 0.50|
|9|NASA explores Mars|5|Shift = 5, Dist = 290.27|Shift = 16, Dist = 311.45|Shift = 9, Dist = 406.22|Shift = 5, Dist = 24.73|Shift = 9, Dist = 28.68|Shift = 1, Dist = 28.79|Shift = 5, Dist = 0.29|Shift = 9, Dist = 0.40|Shift = 1, Dist = 0.41|
|10|Peter Piper picked peppers|9|Shift = 6, Dist = 286.20|Shift = 5, Dist = 418.41|Shift = 13, Dist = 466.62|Shift = 20, Dist = 32.84|Shift = 9, Dist = 36.67|Shift = 13, Dist = 37.67|Shift = 20, Dist = 0.33|Shift = 9, Dist = 0.44|Shift = 13, Dist = 0.48|
|11|Madam, in Eden, I'm Adam|10|Shift = 22, Dist = 265.45|Shift = 10, Dist = 322.25|Shift = 18, Dist = 480.65|Shift = 10, Dist = 34.91|Shift = 18, Dist = 35.49|Shift = 22, Dist = 36.05|Shift = 10, Dist = 0.42|Shift = 18, Dist = 0.44|Shift = 22, Dist = 0.46|
|12|A man, a plan, a canal, Panama|23|Shift = 23, Dist = 347.29|Shift = 6, Dist = 408.13|Shift = 4, Dist = 414.89|Shift = 19, Dist = 43.74|Shift = 4, Dist = 47.03|Shift = 6, Dist = 47.61|Shift = 19, Dist = 0.40|Shift = 4, Dist = 0.51|Shift = 6, Dist = 0.53|
|13|Sphinx of black quartz, judge my vow|24|Shift = 5, Dist = 369.06|Shift = 24, Dist = 375.81|Shift = 10, Dist = 376.56|Shift = 5, Dist = 15.16|Shift = 20, Dist = 15.37|Shift = 24, Dist = 15.92|Shift = 5, Dist = 0.19|Shift = 20, Dist = 0.20|Shift = 24, Dist = 0.22|
|14|Maintaining a balanced diet and regular exercise is crucial for overall well-being and disease prevention.|16|Shift = 16, Dist = 36.75|Shift = 12, Dist = 270.41|Shift = 5, Dist = 309.15|Shift = 16, Dist = 13.22|Shift = 20, Dist = 22.15|Shift = 3, Dist = 22.27|Shift = 16, Dist = 0.11|Shift = 20, Dist = 0.33|Shift = 3, Dist = 0.33|
|15|Four score and seven years ago our fathers brought forth on this continent a new nation, conceived in liberty, and dedicated to the proposition that all men are created equal.|21|Shift = 21, Dist = 13.39|Shift = 1, Dist = 297.69|Shift = 6, Dist = 344.77|Shift = 21, Dist = 6.38|Shift = 8, Dist = 21.71|Shift = 17, Dist = 22.32|Shift = 21, Dist = 0.03|Shift = 8, Dist = 0.33|Shift = 17, Dist = 0.35|
|16|In the heart of ancient Rome, where the grandeur of the Colosseum stood as a testament to the empire's might, gladiators fought for glory and survival. The roar of the crowd echoed through the stone arches as warriors clashed in epic battles. Each combatant, armed with sword and shield, embodied the spirit of courage and determination that defined the era. The games were not merely entertainment but a reflection of the Roman ethos, where strength and valor were celebrated above all else.|13|Shift = 13, Dist = 7.31|Shift = 24, Dist = 240.56|Shift = 12, Dist = 317.58|Shift = 13, Dist = 5.65|Shift = 24, Dist = 22.43|Shift = 0, Dist = 22.70|Shift = 13, Dist = 0.02|Shift = 24, Dist = 0.35|Shift = 0, Dist = 0.36|
|17|The discovery of penicillin by Sir Alexander Fleming in 1928 revolutionized the field of medicine, ushering in the era of antibiotics. This accidental finding, born from a contaminated petri dish, proved to be a turning point in the fight against bacterial infections. Penicillin's ability to inhibit the growth of harmful bacteria offered a lifeline to countless individuals suffering from previously untreatable diseases. Fleming's work laid the foundation for future advancements in pharmacology, saving millions of lives and transforming healthcare practices worldwide|17|Shift = 17, Dist = 14.49|Shift = 4, Dist = 426.60|Shift = 23, Dist = 444.73|Shift = 17, Dist = 8.40|Shift = 21, Dist = 19.64|Shift = 6, Dist = 20.46|Shift = 17, Dist = 0.05|Shift = 21, Dist = 0.29|Shift = 6, Dist = 0.32|

The first few texts are very short texts or repetitions of very short texts. While they appear in the top 3 of possible shifts they are almost always on 2^nd^ or 3^rd^. Of note is text #5 which is a concatenation of text #2 and #4 which cannot be deciphered, but #5 can. Also #6 can be deciphered by all functions even though it's short, this is because it contains mostly e and t which are the most common letters in English. Another noteworthy observation is the accuracy of the Chi-squared function which for short texts like #7, #12 and #8 it is the only one to decipher them, while for #11 it is the only one not to. 

When we get to the longer texts we see the functions being able to decipher them consistently. While for the shorter texts the top three shifts had their distances close to one another, for longer texts the first shift is clearly the right answer.

## Conclusion

For short texts Chi-squared is the most accurate even though it has cases where it's the only one wrong. There are also cases where all of the functions are wrong so different solution might be better. 

For long texts all distance functions work perfectly so frequency analysis is the best to use.