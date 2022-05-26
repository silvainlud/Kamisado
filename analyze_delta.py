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
    plt.xlabel("Niveau des blancs")

    ax.set_ylim(0, 100)
    plt.ylabel("Nombre de victoire pour les blancs")
    ax.grid()
    h1, l1 = ax.get_legend_handles_labels()
    ax1 = ax

    ax = ax.twinx()
    df.plot(y=["diff"], use_index=True, ax=ax, color="tab:green")
    ax.get_legend().remove()
    plt.ylabel("Différence de score")

    h2, l2 = ax.get_legend_handles_labels()

    ax1.legend(h1 + h2, ["Nombre de victoire pour les blancs", "Différence de score"], loc=2)

    plt.title(
        "Nombre de parties gagnées par les blancs en fonction du\nniveau des blancs, avec les noirs au niveau " + str(
            levelWhite))
    plt.savefig("lvl " + str(levelWhite) + ".png")
