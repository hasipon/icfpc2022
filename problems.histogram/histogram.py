from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--image", type=str)
parser.add_argument("--output", type=str)

args = parser.parse_args()

img = Image.open(args.image)

fig, axes = plt.subplots(4, 1)
a = np.array(img)

for i in range(4):
    c = a.T[i]
    axes[i].hist(c.flatten(), bins=np.arange(256 + 1))

plt.tight_layout()
plt.savefig(args.output)
