# BidFX C++ API Usage

## About BidFX

BidFX is the market leading provider of electronic trading solutions for
the global FX marketplace. BidFX has addressed the challenges of the FX
market head on by introducing a complete suite of negotiation protocols
which include: auto-routing, streaming, request-for-quote, voice and
algo-trading via a cloud-based trading platform incorporating
best-execution. Clients have access to a cutting edge, broker neutral,
Execution Management System (EMS) backed by a hub to all major bank's
algo suites. Read about all of our products at
[http://www.bidfx.com.](http://www.bidfx.com.)

## Introduction

This document describes the usage of the BidFX product offering for C++.
Liquidity Providers (LPs) publish their price quotes into the BidFX
platform using the FIX protocol. The platform ingress flow involves
billions for FIX messages per day. BidFX provision for this purpose
high-bandwidth, cross-connect circuits in the key FX data centres:
London (LD4), New York (NY4) and Tokyo (TY3). The BidFX C++ API makes
use of highly optimised, bespoke binary protocols to deliver realtime
quotes from your LPs directly to you in a workable format. The BidFX
delivery mechanism is approximately 80 times more efficient than the FIX
protocol and we publish to you only those quotes that are requested via
a publish and subscribe paradigm.

## Programming Language

The C++ API is written purely in C++17. All of the coded examples below
are also presented in C++.

The API makes use of a publish-subscribe paradigm in which clients
register for price updates by subscribing on subjects. Subjects identify
individual products. Liquidity providers publish streams of real-time
prices against large numbers of subjects. The BidFX price service
matches client's subscription subjects against published subjects and
forwards on to each client only those price updates that match their
subscriptions.

The namespace BidFX.Public.API.Price contains all of the classes,
methods and events that are necessary to subscribe to pricing from a
number of pricing services:

 - Create a connection to our externally accessible pricing service.
 - Subscribe to instruments for multiple price providers.
 - Receive status and/or real-time price updates on each subscription.
 - Unsubscribe from price updates.

## OS Specific Notes


In order to use this library you will need to install ZLib.

**Ubuntu:** Install zlib1g-dev by running
`sudo apt-get install zlib1g-dev` in a terminal.

**Mac OS X:**  Install xcode command line tools by running
`xcode-select --install` in a terminal.

**Windows (cygwin):** Install the zlib-devel package.


## Configuring your IDE (CLion)

- File -> Settings -> Build, Execution, Deployment -> Toolchains
	- Configure the environment to target your cygwin install
	`C:\cygwin64`.
- File -> Settings -> Build, Execution, Deployment -> CMake
	- Configure the debug profile.
		- Set the following CMake options `-DCMAKE_BUILD_TYPE=Debug
		-Dtest=TRUE -Dgtest_disable_pthreads=ON`


## Guides on Usage

### Creating a Session

When using the API the main class you create is called a `Session`, this
can then be configured to work with your username, password and host
etc.

```cpp
std::unique_ptr<Session> session_ = PublicApi::CreateSession();
session_->SetHost("ny-tunnel.uatdev.tradingscreen.com")
	    .SetUsername("demo_username")
	    .SetPassword("password123")
	    .SetDefaultAccount("FX_ACCT")
	    .SetApi("Example API")
	    .SetApiVersion("1.0")
	    .SetProductSerialNumber("TEST_PRODUCT_0012345");
```

It doesn't matter what order you configure your details, but do ensure
you set all those fields listed as required in the following table:

| Name 			| Description   |     Type      | Default Value |
| ------------- | ------------- | ------------- | ------------- |
| Host  | 	The host name of the BidFX point-of-presence (see below) you want to connect to.  | std::string  | -  |
| Username  | The username to authenticate with. This will have to be a valid user for the environment / host.  | std::string  | -  |
| Password  | The password of the selected user.  | std::string  | -  |
| DefaultAccount  | 	The default buy side account use for booking FX orders.  | std::string  | -  |
| Application  | 	The name of the application being used to connect to the server. Used for client administration.  | std::string  | -  |
| ApplicationVersion  | The version number of the application. Used for client administration  | std::string  | -  |
| ProductSerialNumber  | The serial number assigned by BidFX for the product. Used for product licensing purposes.  | std::string | -  |


### Pricing

#### Get the Price Manager

Once you have created your `Session` with the correct properties you can
create a price manager which you can use to setup callbacks.

```cpp
std::unique_ptr<Pricing> pricing_ = std::unique_ptr<Pricing>(&session_->pricing());
```

This will initiate the connection to the price services and await
subscriptions from you. If you encounter an error at this point then
this is most
likely a result of a mis-configuration of one of more of the above
properties.

#### Registering for Price Updates from Subscriptions

Real-time price updates are forwarded to the client application via C++
function pointers. To register for price updates, you create a callback
method (event delegate) to handle the published events. The method
signature should be as follows:

```cpp
void OnPriceUpdate(PriceUpdateEvent& event);
```

You then associate your callback with the event mechanism by creating a
pointer to that function and passing that function pointer as an
argument to the pricing manager. For example:

```cpp
// Define an event delegate for handling price update events.
void ApiExample::OnPriceUpdate(PriceUpdateEvent& event)
{
    std::cout << "Price Callback Triggered" << std::endl;
}

// Create a function pointer to be triggered when new price ticks are received for subscribed subjects
std::function<void(PriceUpdateEvent&)> price_update_callback =
                        [this] (PriceUpdateEvent &event) { OnPriceUpdate(event); };

// Register the callback with the session's price update event mechanism.
pricing_->SetPriceUpdateEventCallback(price_update_callback)
```

#### Registering for Status Updates from Subscriptions

If the price service cannot provide a price for a subject for some
reason, it will publish a subscription status event against the problem
subject.
Again the standard C++ function pointer is employed to manage these
events. To register for status updates, you create a callback method
(event delegate) to handle the event. The method signature should be as
follows:

```cpp
void OnSubscriptionStatus(SubscriptionStatusEvent& event);
```

We recommend that all applications register for subscription status
events. For example:

```cpp
// Define an event delegate for handling status updates.
void ApiExample::OnSubscriptionStatus(SubscriptionStatusEvent& event)
{
    std::cout << "Subscription Callback Triggered" << std::endl;
}

// Create a function pointer to be triggered whenever a subject
// for some reason has encountered a change of state
std::function<void(SubscriptionStatusEvent&)> subscription_callback =
                        [this] (SubscriptionStatusEvent &event) { OnSubscriptionStatus(event); };

// Register the callback with the session's subject status update event mechanism.
pricing_->SetSubscriptionStatusEventCallback(subscription_callback)
```

#### Registering for Status Updates from the Price Provider

The provider of pricing can encounter state changes, such as if the
internet were to go down. These events will trigger a
SubscriptionStatusEvent
which can be received from a callback you can register.

```cpp
// Define an event delegate for handling status updates.
void ApiExample::OnProviderStatus(ProviderStatusEvent& event)
{
    std::cout << "Provider Status Update: << event.Describe();
}

// Create a function pointer to be triggered whenever
// a subject has for some reason encountered a change of state
std::function<void(SubscriptionStatusEvent&)> subscription_callback =
                        [this] (SubscriptionStatusEvent &event) { OnSubscriptionStatus(event); };

// Register the callback with the session's subject status update event mechanism.
pricing_->SetSubscriptionStatusEventCallback(subscription_callback)
```

#### Check if the Pricing Connections are Ready

To check if the session is ready for use, use the Ready property of
`Pricing`.

```cpp
if (pricing_.IsReady())
{
    // the session is ready
}
else
{
    // the session is not ready
}
```

#### Wait for the Pricing Connections to be Ready

It is not necessary to wait until the pricing connections are ready
before making subscriptions but some applications need this capability.
Should
you wish to wait until the pricing connections are ready, then you can
call the method `WaitForSubscriber` on the price session. This method
takes
a `std::chrono::milliseconds` as a timeout interval. If the pricing
connection do not become ready within the given time interval it throws
a `TimeoutException`. In the case of failure you might then take the
opportunity to output the status of each connection - accessible via
`ProviderProperties` - to find out why that connection failed to ready
itself.

For example:
```cpp
try
{
    subscriber_ = pricing_->WaitForSubscriber(std::chrono::milliseconds(10000));
}
catch (TimeoutException& e)
{
    std::cout << "[ERROR] " << e.what();
}
```

#### Creating a Subject

A `BidFX.Public.API.Subject.Subject` is immutable and therefore cannot
be edited once created - we have therefore provided a builder class
`(BidFX.Public.API.Subject.SubjectFactory)` to allow for easy subject
creation.

Below are some examples on how to create subjects in different ways:

```cpp
// To create a subject from scratch using the SubjectFactory, first you must provide UserInfo
UserInfo* user_info = new BasicUserInfo("ln-tunnel.uatprod.tradingscreen.com", "fbennett",
                                        "FX_ACCT", "api_test", "1", "XYZCODE123");

SubjectFactory subject_factory = SubjectFactory(user_info);
Subject new_subject1 = subject_factory.fx().stream().spot()
        .currency_pair("EURGBP")
        .currency("EUR")
        .quantity(1000000)
        .liquidity_provider("CSFX")
        .CreateSubject()

// To create a subject using the details provided by your subscriber object
Subject new_subject2 = subscriber_->subjects().fx().stream().forward()
        .liquidity_provider(lp)
        .currency_pair("EURUSD")
        .currency("EUR")
        .quantity(1000000)
        .tenor(Tenor::IN_1_MONTH)
        .CreateSubject()
```

##### Required Fields and Values

**ALL** deal types:

| Field | Value | Notes |
| ----------- | ------------- | ------------- |
| BuySideAccount | | Either a BuySideAccount (your account with BidFX to attach the order to) or the name of an AllocationTemplate must be specified. |
| AllocationTemplate | | |
| AssetClass | Fx | |
| Currency | | |
| Quantity | | Also AllocQuantity for each BuySideAllocAccount specified |
| RequestFor | Stream or Quote | |
| LiquidityProvider | | |
| DealType |Spot, Swap, Outright, NDS, or NDF | |
| Symbol | AAABBB | Symbol to subscribe to eg "USDJPY" |
| Tenor | Spot, 1W, 2W, 3W, 1M, 2M, 1Y, 2Y, etc. | User can send Tenor or SettlementDate to same effect.  If sending both, the SettlementDate will take precedence. |
| SettlementDate | YYYYMMDD | |
| Level | 1 | only level 1 requests are supported by the API |
| NumAllocs | | If any BuySideAllocAccounts are specified |

**SWAP** deal types:

| Field | Value | Notes |
| ----------- | ------------- | ------------- |
| FarTenor | 1W, 2W, 3W, 1M, 2M, 1Y, 2Y, etc. | User can send FarTenor or FarSettlementDate to same effect |
| FarSettlementDate | YYYYMMDD | |
| FarCurrency |  | Should be the same as Currency |
| FarQuantity | | Also FarAllocQuantity for each BuySideAllocAccount specified |

**NDS** deal types:

| Field | Value | Notes |
| ----------- | ------------- | ------------- |
| FarTenor | 1W, 2W, 3W, 1M, 2M, 1Y, 2Y, etc. | User can send FarTenor or FarSettlementDate to same effect |
| FarSettlementDate | YYYYMMDD | |

##### Valid Tenors

`TOD, TOM, SPOT, SPOT_NEXT`

`1W, 2W, 3W`

`1M … 11M`

`1Y, 2Y, 3Y`

`IMMH, IMMM, IMMU, IMMZ, BD`

#### Subscribing and Unsubscribing

To subscribe or unsubscribe from prices you can call the `Subscribe` and
`Unsubscribe` methods of the price subscriber. These both accept a
`BidFX.Public.API.Subject.Subject` to operate on.

```cpp
Subject subject1 = subscriber_->subjects().fx().stream().spot()
                .liquidity_provider(lp)
                .currency_pair("EURUSD")
                .currency("EUR")
                .quantity(1230080.11)
                .CreateSubject();

// Subscribe to a subject
subscriber_->Subscribe(subject1);

// Unsubscribe from the subject.
subscriber_->Unsubscribe(subject1);
```

Subscriptions are made asynchronously in the background. The `Subscribe`
method will provide no immediate feedback on the success of
otherwise of its action. The client will however quickly receive
feedback on the subscription via one of the event delegate callback
methods. If there is a problem then you will receive a
`SubscriptionStatus` event. If a price was obtained then you will
receive a `PriceUpdate` event. Price updates have an implied status
of `SubscriptionStatus.OK`. Note that it is not uncommon for some price
feeds to initially publish `SubscriptionStatus.PENDING`, to notify the
receipt of a subscription, soon followed by a price update once the data
becomes available from its own remote feed.

`Subscribe` has an optional argument, `auto_refresh`. Setting this to
true will cause the subscription to automatically resubscribe if the
subscription is closed without `Unsubscribe` being called on the
subject, such as a Quote subscription expiring.

#### Price Update Events

Upon receiving a price update in the form of `PriceUpdateEvent`, you'll
want to access the data inside of it. There are three properties:
- A `Subject` got by the Subject property gives the subject of the
subscription the status update refers to.
- An `std::map<std::string, PriceField>` called `AllPriceFields`
containing all current price fields for the instrument.
- An `std::map<std::string, PriceField>` called `ChangedPriceFields`
containing only those fields which changed in this most recent update.

The `ChangedPriceFields` are always a subset of `AllPriceFields` and in
some cases, such as the initial snapshot, they are identical. Often GUI
applications make use of the `ChangedPriceFields` to flash the fields
that have just updated.

##### Price Maps

The price map is `std::map<std::string, PriceField>`, the string being
the field name, while the PriceField itself contains the type and value
of that field. With this map of field names and field values, you can
enumerate over all of the name-value pairs. You can also directly access
individual fields by their name. Field names are just strings with short
camel-case word list. All of the likely published field names are defined
as string constants in the class called
`BidFX.Public.API.Price.FieldName`. To avoid simple typing errors you
should use these constants for accessing specific fields from the map.
For example:

```cpp
using bidfx_public_api::price::FieldName;

// Your event delegate for price updates
void ApiExample::OnPriceUpdate(PriceUpdateEvent& event)
{
    // You can get either AllPriceFields or ChangedPriceFields
    // for the purpose of this example I will get AllPriceFields
    std::map<std::string, PriceField> price = event.GetAllPriceFields();

    // To enumerate over all key-pairs you can call:
    for (std::map<std::string, PriceField>::iterator iter = price.begin(); iter != price.end(); ++iter)
    {
        std::string next_field_name = iter->first;
        PriceField next_price_field = iter->second;

        // You can check which type a particular price field is like this:
        switch(next_price_field.GetType())
        {
            // When you know what type a price field is you can do this to retrieve the value;
            case PriceField::INTEGER:
            {
                int32_t integer_val = next_price_field.GetInt();
                break;
            }
            case PriceField::LONG:
            {
                int64_t long_val = next_price_field.GetLong();
                break;
            }
            case PriceField::DOUBLE:
            {
                double double_val = next_price_field.GetDouble();
                break;
            }
            case PriceField::STRING:
            {
                std::string string_val = next_price_field.GetString();
                break;
            }
            case PriceField::NOTHING:
                break;
        }
    }

    // Accesses a specific PriceField (will throw out_of_range exception if field does not exist)
    PriceField bid = event.GetAllPriceFields().at(FieldName::Bid);

    // To access a specific field you can call:
    event.GetAllPriceFields().at(FieldName::AccruedInterest);
}
```

##### Field Names

The table below defines some of the more common field names that can be
returned from a subscription. These are defined as string constants in
the class called `BidFX.Public.API.Price.FieldName` for convenience.

| Field Name | Stream & Quote |
| ----------- | ------------- | 
|       Bid      |       yes        |
|       Ask      |         yes      |
|       BidTime      |         yes      |
|       AskTime      |         yes      | 
|       OriginTime      |       yes        | 
|       PriceID      |         yes      | 
|       BidSpot      |          yes     |
|       AskSpot      |         yes      | 
|       BidForwardPoints      |  yes             |
|       BidForwardPoints      |     yes          | 
|       AskForwardPoints      |        yes       |
|       SystemTime      |       yes        |


#### Subscription Status Events

Upon receiving a status update in the form of a
`SubscriptionStatusEvent`, you'll want to access the data inside of it.
There are three properties:

- A `Subject` called Subject gives the subject of the subscription the
status update refers to.
- A `SubscriptionStatus` containing an enum value that defines the
status of the subscription.
- A `std::string` containing a more detailed reason of the change in
status.

The data can be accessed as follows:

```cpp
// Your event delegate for subscription status events
void ApiExample::OnSubscriptionStatus(SubscriptionStatusEvent& event)
{
    Subject subject = event.GetSubject();
    SubscriptionStatus status = event.GetStatus();
    std::string reason = event.GetExplanation();
}
```

Most subscription status' result from temporary issues that can be
resolved by the price service. For example, if a connection goes down
and fails over then the API will publish a recoverable status code such
as `SubscriptionStatus::STALE` and then quickly reconnect and resume
publishing prices again.

Other status codes are more serious and tend to be semi-permanent in
nature; examples are, bad symbols, invalid price source, user
authentication or authorisation error. These are normally unrecoverable
without some admin user intervention followed by re-subscribing. That
said, in a long running application we need every type of error the be
potentially recoverable in time without the need to restart the
application. So if a subscription comes back with a server status code,
such as the liquidity provider's remote feed not running, the API will
automatically resubscribe periodically in an attempt to recover. This
re-subscription interval is, by default, a relatively long 5 minutes to
prevent overly stressing the server with bad subscriptions. The time
interval can however be configured if required by setting the session
property `SubscriptionRefreshInterval`.

#### Connection Status Events

When the price session is started it will create and start connections
to each of it's providing services. Each service connection will be
initially in a down state with the status
`ProviderStatus.TemporarilyDown`. As connections come online or go
offline they send status updates in the form of `ProviderStatusEvents`.
Clients can register callbacks to receive notification of these events.

```cpp
// Define a callback for handling ProviderStatusEvents
void ApiExample::OnProviderStatus(ProviderStatusEvent& event)
{
    Log.Info("provider status: " +
             providerStatusEvent.Name + " changed status from " +
             providerStatusEvent.PreviousProviderStatus
             + " to " + providerStatusEvent.ProviderStatus
             + " because: " + providerStatusEvent.StatusReason);
}

// Register callback with the Pisa session
std::function<void(ProviderStatusEvent&)> provider_callback =
                        [this] (ProviderStatusEvent& event) { OnProviderStatus(event); };

pricing_.SetProviderStatusEventCallback(provider_callback);
```

#### Accessing provider properties

It is possible to access the current properties of each provider by
calling `GetProviderProperties` on the price session. This gives the
current status of each provider. For example:

```cpp
for (ProviderProperties& properties : pricing_->GetProviderProperties())
{
    std::cout << properties.GetName() << " is " << properties.GetStatusString();
}
```

#### Stop the price session

When you are finished with the pricing session you should stop the
session to free up its resources by calling `Stop`. For example:

```cpp
// Stop the session.
pricing_->Stop();
```

### Logging

The API makes use of the logging framework spdlog for logging important
activity and errors. As a guide we log high volume messages, such as
price ticks, at `debug` level so they are easily disabled via
configuration. Server connection attempts are logged at info level, as
are subscriptions. Connections failures are logged at `warn` level.

We suggest setting the log level to `info` in a production system to
keep the level of logging manageable. If you choose to enable debug
logging in production then there will be a performance impact when
prices update quickly.

Logging is configured using the LoggerFactory class.

Log messages are sent to sinks. There are several sinks available,
including Stdout, DailyFile, and RotatingFile. Multiple sinks can be
configured, with independent logging levels. When adding a log sink
(described below), a shared pointer to the sink is returned. This
pointer allows you to set the log level using
`set_level(spdlog::level::level_enum level)`. The flush severity can
also be configured using flush_on (spdlog::level::level_enum level).
This setting will force the logger to flush its output when a log
message of a certain level is received. Alternatively, the log level and
flush severity can be set globally using
`LoggerFactory::SetLoglevel(spdlog::level::level_enum level)` and
`LoggerFactory::SetFlushSeverity(spdlog::level::level_enum level)`.
These methods will overwrite  any individual configurations.

#### Available Sinks

##### DailyFileLogger

spdlog::sink_ptr LoggerFactory::AddDailyFileLogger(std::string base_filename)
Writes to a file that rolls automatically at midnight
base_filename - the base name of the file to write to, including file
path. **The folder structure must already exist**

##### RotatingFileLogger

spdlog::sink_ptr LoggerFactory::AddRotatingFileLogger(std::string base_filename, size_t max_size, size_t max_files)
Writes to a file that rolls automatically when the file reaches a
certain size base_filename - the base name of the file to write to,
including file path. **The folder structure must already exist**

max_size - the maximum size a log file should reach before rolling

max_files - the maximum number of files to retain

##### StdoutLogger


spdlog::sink_ptr LoggerFactory::AddStdoutLogger(bool colors)
Writes to stdout
colors - whether color coding should be used on the log severity

##### OStreamLogger

spdlogg:sink_ptr LoggerFactory::AddOstreamLogger(std::ostream& ostream)
Writes to an output stream.
ostream - the output stream to write to

##### NullLogger

spdlog::sink_ptr LoggerFactory::AddNullLogger()
Throws away logs. Can be used for debugging or reference implementation.

---

To print to the log streams, you must create a logger instance. This is
done by using the `LoggerFactory`. The `LoggerFactory` has a static
function called `GetLogger(std::string)`, which takes a string to help
identify in the log file which logger instance printed which statements.

```cpp
// This shows how to create a new logging instance with the logger ID of "ApiExample"
std::shared_ptr<spdlog::logger> logger = LoggerFactory::GetLogger("ApiExample");

// Printing to the log file at various levels of escalation
logger->trace("this is a trace log message");
logger->debug("this is a debug log message");
logger->info("this is an info log message");
logger->warn("this is a warning log message");
logger->error("this is an error log message");
```

## Points of Presence

BidFX provide a number of points-of-presence (POPs) around the world via
which clients may connect to our global price service:

### Production

Clients are encouraged to connect to their geographically closest access
point for minimum latency.

| City 			| Environment   |     DNS name      |   IP    | Port |
| ------------- | ------------- | ------------- | ------------- | ------------- |
| New York  |  PROD  | ny-tunnel.prod.tradingscreen.com  | 199.27.81.88 | 443 |
| London  |  PROD  | ln-tunnel.prod.tradingscreen.com  | 199.27.86.88 | 443 |
| Paris  |  PROD  | pa-tunnel.prod.tradingscreen.com  | 199.27.83.88 | 443 |
| Tokyo  |  PROD  | to-tunnel.prod.tradingscreen.com  | 199.27.85.88 | 443 |
| Hong Kong  |  PROD  | hk-tunnel.prod.tradingscreen.com  | 199.27.84.88 | 443 |

### Test environments


We provide a test environment, feeding UAT market data. POPs for the
test environments are available in New York and London, so some clients
may experience additional latency.

| City 			| Environment   |     DNS name      |   IP    | Port |
| ------------- | ------------- | ------------- | ------------- | ------------- |
| New York  |  UAT  | ny-tunnel.uatprod.tradingscreen.com  | 199.27.81.99 | 443 |
| London  |  UAT  | ln-tunnel.uatprod.tradingscreen.com  | 199.27.86.91 | 443 |