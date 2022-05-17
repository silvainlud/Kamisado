import pandas as pd
import matplotlib.pyplot as plt

dataframe = pd.read_csv("result.csv")

dataframe["diff"] = dataframe["meanBlack"] - dataframe["meanWhite"]
dataframe["diff"] = dataframe["diff"]

for levelWhite in range(1, 10):
    df = dataframe[dataframe["levelWhite"] == levelWhite]

    fig, ax = plt.subplots()
    df = df.groupby(["levelBlack"]).sum()

    df.plot(y=["whiteWin"], use_index=True, ax=ax, color="tab:red")

    ax.set_ylim(0, 100)
    ax.legend("Nombre de victoire pour les blancs", loc=0)
    ax.grid()

    ax = ax.twinx()
    df.plot(y=["diff"], use_index=True, ax=ax, color="tab:green")
    ax.legend("Différence de score", loc=0)

    plt.title("lvl " + str(levelWhite))
    plt.savefig("lvl " + str(levelWhite) + ".png")
