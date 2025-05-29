import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

URL = 'ion_temp120.csv'
heater1=pd.read_csv(URL1, sep='\t',header=5)
heater1.head()
Va1 = np.array(heater1['Va(V)_1']) # accelerating voltage vector
I1 = np.array(heater1['Ia(E-12 A)_1']) # Current vector
plt.figure()
plt.plot(Va1,I1)
plt.title('Heater 1')

#linear regression
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import PolynomialFeatures
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import PolynomialFeatures

# Create a polynomial regression model
degree = 2  # Degree of the polynomial          