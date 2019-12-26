# Points of Presence

BidFX provides several network Points-Of-Presence (POPs) around the world
where our clients may connect to access our pricing and trading service.
TLS secures all connections over a single port which is sufficient for both pricing and order flow.

## Production

The table below lists the available productions POPs.

| City      | Environment  |     DNS name                     | IP           | Port |
| --------- | ------------ | -------------------------------- | ------------ | ---- |
| New York  | PROD         | ny-tunnel.prod.tradingscreen.com | 199.27.81.88 |  443 |
| London    | PROD         | ln-tunnel.prod.tradingscreen.com | 199.27.86.88 |  443 |
| Paris     | PROD         | pa-tunnel.prod.tradingscreen.com | 199.27.83.88 |  443 |
| Tokyo     | PROD         | to-tunnel.prod.tradingscreen.com | 199.27.85.88 |  443 |
| Hong Kong | PROD         | hk-tunnel.prod.tradingscreen.com | 199.27.84.88 |  443 |

Clients are encouraged to connect to their geographically closest access
point for minimum latency.


## User Acceptance Testing

BidFX also provides dedicated User Acceptance Test (UAT) environments
which clients can uses to test and certify their software before going into production.
The table below lists the available UAT POPs.

| City      | Environment | DNS name                            |   IP         | Port |
| --------- | ----------- | ----------------------------------- | ------------ | ---- |
| New York  | UAT         | ny-tunnel.uatprod.tradingscreen.com | 199.27.81.99 |  443 |
| London    | UAT         | ln-tunnel.uatprod.tradingscreen.com | 199.27.86.91 |  443 |

UAT pricing subscriptions are routed to LP test sessions.
These will quote prices similar to current market rates but will not compare well with production data,
UAT orders are routed to the BidFX market simulator in New York.
Clients located elsewhere will experience additional latency on their orders as
they route to New York for handling.
