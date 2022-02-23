# Common Problems

## On-boarding Issues

### Credentials

Clients need to obtain valid user credentials to connect to BidFX services via the API.
Please don't access the API using personal credentials.
BidFX on-boarding staff usually provide dedicated credentials for API usage.
Credentials will differ from UAT to production so be sure not to confuse the two.

If you create a `Session` using an incorrect username or password,
or you fail to configure credentials, then the API will return a suitable error soon after starting.

### API Product

A valid username is insufficient by itself to access the API.
The user also requires a backend *product assignment* for API access.
Your BidFX account manager will set this up for you.
There may be a usage charge associated with API accesss.

### Booking Account

FX subscriptions require a booking account. 
Prices, session routing and entitlements can differ from account to account.
If required, a separate account can be set for each subscription
but most commonly a single default account is used for all API subscriptions.
The default account is configured on the API `Session` interface.
Please verify with your account manager to ensure you are providing the correct account to the API.

### LP Relationships and Entitlements

You might find that you can connect to the price service, receive some pricing but not get results for all subscriptions; perhaps some LPs or currency pairs are absent. This situation is most likely due to the assigned customer relationships or entitlements for the user account. Talk to you account manager to ensure that all LPs have been fully on-boarded.

## Connectivity Issues

Many first time users of the API will experience connectivity issues due to firewalls, content filtering or network accelerator devices managed by their network support team.


### Firewalls

A firewall is generally in place at most client organisations to
provide basic network security.
It is often necessary for API users to request their local network team to
white-list an IP address or open
a port in the firewall to the required BidFX [POP](POP.md).

It is possible to test the connectivity to BidFX using the
`ping` and `telnet` commands as follows:

```sh
$ ping api.ld.bidfx.biz
PING api.ld.bidfx.biz (64.52.164.80): 56 data bytes
64 bytes from 64.52.164.80: icmp_seq=0 ttl=58 time=1.175 ms
64 bytes from 64.52.164.80: icmp_seq=1 ttl=58 time=1.076 ms

$ telnet api.ld.bidfx.biz 443
Trying 199.27.86.91...
Connected to api.ld.bidfx.biz.
Escape character is '^]'.
```

Consult your network team if either command returns an error
or hangs without returning.

### Network devices

Security-conscious organisations commonly deploy network devices to inspect packets, shape traffic or optimise bandwidth.
Such devices can corrupt the message flow to BidFX and cause a protocol marshalling error, quickly followed by a session disconnect.
Network security devices will often allow the initial connection through to BidFX but
interfere subsequently with either the price protocol or the TLS security layer.
Packet inspection might, for example, appear to the API as a man-in-the-middle attack.
Other packet inspection devices prevent WebSocket upgrades by filtering the HTTP headers, either intentionally or unintentionally.

The commonly observed symptom in these cases is a read timeout or an SSL handshake error, received soon after establishing the connection.
The solution to all of these issues is to request your company's network team
to bypass the offending device(s) for the BidFX IP addresses and port.


## Subscription Limits

The BidFX price service limits the number of simultaneous price subscriptions that an API may make. We restrict subscriptions, both to protect the price service from excessive use and to guarantee a high quality of service for all users. The default subscription limits allow hundreds of price subscriptions and are sufficient and appropriate for most users.
Too many subscriptions can generate substantial amounts of network traffic,
causing bandwidth saturation and heavy CPU load on the client host.
If an overworked client application becomes a *slow consumer* then it will experience excessive latency as a result.

We recommend all API users to close subscriptions that are no longer required
to minimise network load.


## Increasing Limits

BidFX may provide a licence key that grants a larger than average subscription limit for demanding client applications.
The licence key is provided to the API when creating the `Session` to BidFX.
To qualify for a higher subscription limit, the client will need to:

 - have sufficient CPU capacity to comsume the increased price update load,
 - run their application physically close to the source of liquidity,
 - have a good quality, high bandwidth network connection (ideally a data centre cross-connect to BidFX).

There may be an additional service charge for the extra subscription limit.
Ask your BidFX sales representative for price details.
