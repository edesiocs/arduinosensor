# Table of Contents #

# Introduction #

Add your content here.


# Details #

Twitter has many ways to authenticate a user. For this project, since the device doesn't change we are using "Single-user OAuth". Using that method allows use not to not need to implement the full OAuth token acquisition handshakes.

All you need is:-

Consumer key
Consumer secret key
Access token
Access token secret
In order to do so, signup for a new Twitter account that you plan to use for the device. Then head over to https://dev.twitter.com/.

Once there, go to "My Applications" that is listed under the account name and "Create a new application".

Fill up what is needed then head to "Settings" and modify "Application type" to "Read and Write". Then go to "Details" and create an Access Token.

After all is done, take note of the Consumer key, Consumer secret key, Access token and Access token secret.

With this information, you can then tweet directly from python. Here's the code:-

```
  import twitter

  api = twitter.Api(consumer_key='key', consumer_secret='key', access_token_key='key', access_token_secret='key') 
  api.PostUpdate("Hello world from Python!")
```