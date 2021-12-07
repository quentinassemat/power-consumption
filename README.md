# Prediction of Power Consumption

I conducted this project with Quentin Assémat under the supervision of Professor Steve Oudot.

The goal was to analyze the data available for the power consumption of a household, in order to identify consumption patterns and establish their correlation with meteorological data. We detected apparent seasons with the *k*-means clustering method and tested the presence of known distributions in the hourly consumption with Kolmogorov-Smirnov tests. We estimated and back-tested the power consumption with meteorological data using linear and *k*-NN regressions.

The data structures and machine learning algorithms were implemented in C++ for efficiency. The preprocessing and visualization parts were done with Jupyter notebooks.
