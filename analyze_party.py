import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

dataframe = pd.read_csv("result_7vs5.csv")
dataframe["countChange"] = dataframe["countUp"] + dataframe["countDown"]

fig, ax = plt.subplots()
dataframe.plot(y=["levelBlack"], use_index=True, ax=ax, color="tab:green", legend="Niveau des noirs")
dataframe.plot(y=["levelWhite"], use_index=True, ax=ax, color="tab:red", legend="Niveau des blancs")
dataframe.plot.bar(y=["countChange"], use_index=True, ax=ax, color="tab:blue", legend="Nombre de changements", rot=0)
plt.xticks(np.arange(min(dataframe.index), max(dataframe.index) + 1, 25.0))
plt.xlabel("Numéro d'exécution")
plt.ylabel("Niveau")
plt.title("Le niveau en fonction du numéro d'exécution")
plt.legend(["Niveau des noirs", "Niveau des blancs", "Nombre de changements"])
plt.ylim(0, 9)
plt.savefig("res_lambda_7vs5.png")

fig, ax = plt.subplots()
mylabels = ["Nombre de victoires des blancs", "Nombre de victoires des noires"]
plt.pie([489, 511], labels=mylabels)
plt.savefig("res_lambda_7vs5_pie.png")
