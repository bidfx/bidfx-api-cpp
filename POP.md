# Points of Presence

BidFX provides several network Points-Of-Presence (POPs) around the world
where our clients may connect to access our pricing and trading service.
TLS secures all connections over a single port which is sufficient for both pricing and order flow.

## Production

The table below lists the available productions POPs.

| City      | Environment  |     DNS name         | IP           | Port |
| --------- | ------------ | -------------------- | ------------ | ---- |
| London    | PROD         | api.ld.bidfx.com     | 64.52.164.90 |  443 |
| New York  | PROD         | api.ny.bidfx.com     | 64.52.165.90 |  443 |
| Tokyo     | PROD         | api.ty.bidfx.com     | 64.52.166.90 |  443 |
| Singapore | PROD         | api.sg.bidfx.com     | 64.52.167.90 |  443 |

Clients are encouraged to connect to their geographically closest access
point for minimum latency.


## User Acceptance Testing

BidFX also provides dedicated User Acceptance Test (UAT) environments
which clients can uses to test and certify their software before going into production.
The table below lists the available UAT POPs.

| City      | Environment | DNS name          |   IP         | Port |
| --------- | ----------- | ----------------- | ------------ | ---- |
| London    | UAT         | api.ld.bidfx.biz  | 64.52.164.80 |  443 |
| Tokyo     | UAT         | api.ty.bidfx.biz  | 64.52.166.80 |  443 |

UAT pricing subscriptions are routed to LP test sessions.
These will quote prices similar to current market rates but will not compare well with production data,
UAT orders are routed to the BidFX market simulator in New York.
Clients located elsewhere will experience additional latency on their orders as
they route to New York for handling.
