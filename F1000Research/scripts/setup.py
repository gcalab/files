from setuptools import find_packages, setup

setup(
    name="CoVMutSeasonal",
    packages=find_packages(where="covmut_seasonal"),
    version="0.1.0",
    description=""" \
        Global Seasonal Analysis on Amino Acid Substitution prevelance
        for the SARS-CoV-2 virus.
    """,
    author="Tre Tomaszewski",
    author_email="jtomasz2@illinois.edu",
    license="MIT",
    python_requires=">=3.6"
)
