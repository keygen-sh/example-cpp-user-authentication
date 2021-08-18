# Example C++ User Authentication

This is an example C++ program for authenticating as a user and using a user
token to interact with the API. It contains a small command line script that
generates a new API token using the user's email and password, and then uses
that token to retrieve the user's profile and to list the user's licenses.

## Running the example

First up, add an environment variable containing your account ID:

```bash
# Your Keygen account ID
export KEYGEN_ACCOUNT_ID="YOUR_KEYGEN_ACCOUNT_ID"
```

You can either run each line above within your terminal session before
starting the app, or you can add the above contents to your `~/.bashrc`
file and then run `source ~/.bashrc` after saving the file.

Next, on macOS, compile the source using `g++`:

```bash
g++ main.cpp -o bin.out \
  -std=c++17 \
  -stdlib=libc++ \
  -lcurl
```

Then run the bin:

```bash
KEYGEN_ACCOUNT_ID=demo ./bin.out
```

## Running on other platforms

We are only including instructions on how to compile and run this example on
macOS. If you'd like to create a PR with instructions for another platform,
such as Windows or Linux, please feel free to open a PR.

## Questions?

Reach out at [support@keygen.sh](mailto:support@keygen.sh) if you have any
questions or concerns!
