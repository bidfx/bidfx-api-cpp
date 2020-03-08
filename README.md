![BidFX-C++-API](https://user-images.githubusercontent.com/2197551/76166473-b92c6e80-6156-11ea-9f25-afb0d06ad4e7.png =640x)

# Public API for C++

[![License](https://img.shields.io/badge/license-Apache%202-4EB1BA.svg)](https://www.apache.org/licenses/LICENSE-2.0.html)

-------

## API Features

The BidFX C++ API is a price API that connects to the BidFX trading platform to subscribe to realtime pricing.
The API supports the following features:

 - FX streaming executable prices (RFS)
 - FX request for quote (RFQ)
 - FX and Futures trading from C++ is available via the REST.

Most users of the API will trade against the received prices.
Trading is achieved by making RESTful requests to post orders via the BidFX REST API.
The REST API is easily accessed using C++.

## About BidFX

BidFX is the market-leading provider of electronic trading solutions for the global foreign exchange marketplace. BidFX has addressed the challenges of
the FX market by introducing a complete suite of negotiation protocols
– which include: auto-routing, streaming, request-for-quote, voice,
algo-trading and best execution – via a cloud-based SaaS trading platform.
BidFX offer clients access to a cutting edge, broker-neutral,
Execution Management System (EMS) backed by a hub to all major bank's
algo suites. You can read about all BidFX products on the
[BidFX Website](https://www.bidfx.com).


## Other APIs

BidFX clients generally access the trading platform via a dedicated User Interface (UI) either on their desktop PC, web browser or mobile
device.
APIs provide a secondary means of accessing the trading platform that can either supplement
the UI or replace it entirely for some use cases, including systematic trading, OMS integration and market intelligence.
BidFX place significant emphasis on API support and therefore
provide a suite of APIs for different high-level programming languages
and data exchange protocols.

You can read about the complete BidFX API range, and their different capabilities,
at [BidFX API Overview](https://www.bidfx.com/apis).


## C++ API

This document describes the BidFX Public API for C++.
The C++ API is written purely in C++17. All of the code examples below
are also presented in C++.
We use the tag *Public* to indicated that,
this API is designed and maintained for public use by BidFX clients.
Being *public* implies a degree of support, API stability and future
compatibility appropriate to client usage.

### Realtime Price feeds

Liquidity Providers (LPs) mostly publish tradable price/quotes into the BidFX
platform using the FIX protocol. The quotes from LPs are firm prices
with an associated price ID that needs to be attached to any order made against the quote.
The BidFX platform consumes billions for FIX messages per day. We provision
high-bandwidth, cross-connect circuits in the main global data centres for this purpose.
BidFX cross-connect where most banks host their price engines, in particular in:

 - London (LD4),
 - New York (NY4) and
 - Tokyo (TY3).

FX quotes are short-lived and LPs reserve the right of **last look**.
A quote usually is good for no more than a few hundred milliseconds.
Network latency between the client application and the LP is therefore
a significant consideration if we want to avoid rejections.
If clients intend to trade directly against price IDs, then it is recommended
that they run their application very close to the source of liquidity
and cross-connect within the same data centre if possible.
Alternatively, clients may route their orders to the BidFX Strategy Server to minimise both rejections and slippage.

The BidFX C++ API implements a highly optimised, bespoke binary protocol to deliver realtime quotes from LPs directly to into a client's application with minimal latency.
The binary delivery mechanism is approximately 80 times more efficient than the FIX protocol.
Furthermore, using the publish and subscribe paradigm, BidFX servers
publish only those quotes that are subscribed to.

## API Set Up

### Git Repository

As part of the BidFX Open Source initiative,
we have published the source code for the BidFX C++ API on the
[BidFX Github Page](https://github.com/bidfx).
You can clone the API with the following command.

```sh
git clone https://github.com/bidfx/bidfx-api-cpp.git
```

### OS Specific Set Up

To use the API, you will need to install the `ZLib` data compression library.

 - **Ubuntu:** Install `zlib1g-dev` by running
`sudo apt-get install zlib1g-dev` in a terminal.

 - **Mac OS X:**  Install the Apple *xcode* command line tools by running
`xcode-select --install` in a terminal.

 - **Windows (cygwin):** Install the `zlib-devel` package.


### Configuring your IDE

For C++ development we recommend an integrated development environment (IDE)
designed for programming in C++.
We use the **CLion** IDE from JetBrains. You can configure CLion as follows.

- File -> Settings -> Build, Execution, Deployment -> Toolchains
    - Configure the environment to target your cygwin install
    `C:\cygwin64`.
- File -> Settings -> Build, Execution, Deployment -> CMake
    - Configure the debug profile.
        - Set the following CMake options `-DCMAKE_BUILD_TYPE=Debug -Dtest=TRUE -Dgtest_disable_pthreads=ON`



### Building With Make

If you prefer to build on the command line, using the makefile,
then the following commands can be used.

```sh
cd public-api-cpp
cmake .
make
```

## API Usage

The Price API makes use of a publish-subscribe paradigm in which clients
register for price updates by subscribing on subjects. Subjects identify
individual products or instruments for which realtime pricing may be obtained.
Liquidity providers publish streams of realtime
prices against large numbers of subjects. The BidFX price service
matches the client's subscribed subjects against the total universe of
published subjects and forwards on to each client only those price updates
that match their subscriptions.

The namespace `BidFX.Public.API.Price` contains all of the classes,
methods and event handlers that are necessary to subscribe to pricing from
several pricing services. The common usage patten for the API involves:

 - Creating a connection to BidFX's externally accessible pricing service.
 - Subscribe to subjects representing instruments from multiple price providers.
 - Receive status and realtime price updates on each subscription.
 - Unsubscribe from price subjects to stop receiving updates.

### Creating a Session

When using the API the main class you create is called a `Session`, this
can then be configured to work with your username, password and host
etc.

```cpp
std::unique_ptr<Session> session_ = PublicApi::CreateSession();
session_->SetHost("ny-tunnel.uatdev.tradingscreen.com")
        .SetUsername("acme_api")
        .SetPassword("secret_password!")
        .SetDefaultAccount("FX_ACCT")
        .SetApplication("ACME OMS")
        .SetApplicationVersion("2.1")
        .SetProductSerialNumber("f2d332674ddfe0b02833979c");
```

It doesn't matter what order you configure your details, but do ensure
you set all those fields listed in the following table:

| Name                 | Description   |     Type      |
| -------------        | ------------- | ------------- |
| Host                 | The hostname of the BidFX [POP](POP.md). The host determines the environment. | std::string  |
| Username             | The username to authenticate with. Must be a valid API user for the environment. | std::string  |
| Password             | The password of the selected user. | std::string  |
| DefaultAccount       | The default buy-side account use for booking FX orders. Prices can vary by account. | std::string  |
| Application          | The name of the client application connecting to the server. Used for client administration/monitoring.  | std::string  |
| ApplicationVersion   | The version number of the client application. Used for client administration/monitoring. | std::string  |
| ProductSerialNumber  | The serial number assigned by BidFX for the product. Used for product licensing purposes. | std::string |


### Using the Pricing Interface

From the main `Session` you can access the `Pricing` interface that is used to register for callbacks.

```cpp
std::unique_ptr<Pricing> pricing_ = std::unique_ptr<Pricing>(&session_->pricing());
```

The above call will initiate the connection to the price services and await subscription requests.
Any error at this point is most likely a result of a misconfiguration of one or more of the above properties.

#### Price Updates Callbacks

The server forwards realtime price updates to the client application via C++ function pointers. To register for price updates, you create a callback method to handle published price events. The method signature should be as follows:

```cpp
void OnPriceUpdate(PriceUpdateEvent& event);
```

You then associate your callback with the event mechanism by creating a
pointer to that function and passing that function pointer as an
argument to the pricing manager. For example:

```cpp
// Define an callback function for handling price update events.
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

#### Subscription Status Callback

If the price service cannot provide a price for a subject for some
reason, it will publish a subscription status event against the problem
subject.
A callback in the form of a C++ function pointer should be provided to handle these
events. The method signature should be as follows:

```cpp
void OnSubscriptionStatus(SubscriptionStatusEvent& event);
```

We recommend that all applications register for and handle subscription status
events. For example:

```cpp
// Define an callback function for handling status updates.
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

#### Provider Status Callback

The price API can support multiple price providers (price servers).
The provider of pricing can encounter state changes, such as if the
connection were to go down. These events will trigger a
SubscriptionStatusEvent
which can be received from a registered callback.

```cpp
// Define an callback function for handling status updates.
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

#### Check Pricing Is Ready

To check if the session is ready for use, use the **Ready** property of the
`Pricing` interface.

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

#### Wait For Pricing To Be Ready

It is not always necessary to wait until the pricing connections are ready
before making subscriptions, but may applications need this capability.
Should you wish to wait until the pricing connections are ready, then you can
call the method `WaitForSubscriber` on the price session. This method
takes
a `std::chrono::milliseconds` as a timeout interval. If the pricing
connection does not become ready within the given time interval, it throws
a `TimeoutException`. In the case of failure, you might then take the
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

The following tenors may be used:

 - Near date tenors: `TOD, TOM, SPOT, SPOT_NEXT`
 - Weekly tenors: `1W, 2W, 3W`
 - Monthly tenors: `1M, 2M … 11M`
 - Yearly tenors: `1Y, 2Y, 3Y`
 - IMM future tenors: `IMMH, IMMM, IMMU, IMMZ`
 - Broken date tenors: `BD` (an explicity settlement date is required).


#### Subscribing and Unsubscribing

To manage price subscriptions call the `Subscribe` and
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

Subscriptions are made asynchronously in the background. The `Subscribe` method will provide no immediate feedback on the success or otherwise of its action. The client will, however, quickly receive
feedback on the subscription via one of the event callback
methods. If there is a problem, then you will receive a
`SubscriptionStatus` event. You will
receive a `PriceUpdate` event each time a price is received. Price updates have an implied status
of `SubscriptionStatus.OK`. Note that it is not uncommon for some price
feeds to initially publish `SubscriptionStatus.PENDING`, to notify the
receipt of a subscription, soon followed by a price update once the data
becomes available from its remote feed.

`Subscribe` has an optional argument, `auto_refresh`. Setting this to
`true` will trigger a subscription refresh whenever the
subscription is terminated prematurely without calling `Unsubscribe`  on the subject (such as a discontinued RFQ).

#### Price Update Events

Upon receiving a price update in the form of `PriceUpdateEvent`, you'll
want to access the data inside of it. There are three properties:
- A `Subject` got by the Subject property gives the subject of the
subscription the status update refers to.
- An `std::map<std::string, PriceField>` called `AllPriceFields`
containing all current price fields for the instrument.
- An `std::map<std::string, PriceField>` called `ChangedPriceFields`
containing only those fields which changed in this most recent update.

The `ChangedPriceFields` are always a subset of `AllPriceFields`, and in
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
`BidFX.Public.API.Price.FieldName`. To avoid simple typing errors, you
should use these constants for accessing specific fields from the map.
For example:

```cpp
using bidfx_public_api::price::FieldName;

// Your callback function for price updates
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

The table below defines some of the more common price field names - defined as string constants in
the class called `BidFX.Public.API.Price.FieldName` for convenience.

| Field Name  | Stream & Quote |
| ----------- | -------------  |
| Bid         |      yes       |
| Ask         |      yes       |
| BidTime     |      yes       |
| AskTime     |      yes       |
| OriginTime  |      yes       |
| PriceID     |      yes       |
| BidSpot     |      yes       |
| AskSpot     |      yes       |
| BidForwardPoints | yes       |
| BidForwardPoints | yes       |
| AskForwardPoints | yes       |
| SystemTime  |      yes       |


#### Subscription Status Events

Upon receiving a status update in the form of a
`SubscriptionStatusEvent`, you'll want to access the data inside of it.
There are three properties:

- A `Subject` gives the price subject of the subscription that the update refers to.
- A `SubscriptionStatus` containing an enum value that defines the
status of the subscription.
- A `std::string` containing a more detailed reason for the change in
status.

The data can be accessed as follows:

```cpp
// Your callback function for subscription status events
void ApiExample::OnSubscriptionStatus(SubscriptionStatusEvent& event)
{
    Subject subject = event.GetSubject();
    SubscriptionStatus status = event.GetStatus();
    std::string reason = event.GetExplanation();
}
```

Most subscription status' result from temporary issues that can be resolved by the price service. For example, if a connection goes down
and fails-over then the API will publish a recoverable status code such
as `SubscriptionStatus::STALE` and then quickly reconnect and resume
publishing prices again.

Other status codes are more severe and tend to be semi-permanent in
nature; examples are, incorrect symbols, invalid price source, user
authentication or authorisation error. These are generally unrecoverable
without some admin user intervention followed by resubscribing. That
said, in a long-running application, we need every type of error the be
potentially recoverable in time without the need to restart the
application. So if a subscription comes back with a server status code,
such as the liquidity provider's remote feed not running, the API will
automatically resubscribe periodically in an attempt to recover. This
re-subscription interval is, by default, a relatively long 5 minutes to
prevent overstressing the server with bad subscriptions. You can change the time interval by setting the session
property `SubscriptionRefreshInterval`.

#### Connection Status Events

Upon starting the price session, the API will create and initiate connections
to each of it's providing services. Each service connection will be
initially in a downstate with the status
`ProviderStatus.TemporarilyDown`. As links come online or go offline, they send status updates in the form of `ProviderStatusEvents`.
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
calling `GetProviderProperties` on the price session. This call gives the
current status of each provider. For example:

```cpp
for (ProviderProperties& properties : pricing_->GetProviderProperties())
{
    std::cout << properties.GetName() << " is " << properties.GetStatusString();
}
```

#### Stop the price session

When finished with the pricing session, you should stop the
session to free up its resources by calling `Stop`. For example:

```cpp
// Stop the session.
pricing_->Stop();
```

### Logging

The API makes use of the logging framework **spdlog** for logging important
activity and errors. As a guide, we log high volume messages, such as
price ticks, at `debug` level, so they omitted from production logs. Server connection attempts are logged at info level, as
are subscriptions. We log connections failures at `warn` level.

We suggest setting the log level to `info` in a production system to
keep the level of logging manageable. If you enable `debug`
logging in production, then there will be a performance impact when
prices update very quickly.

Use the `LoggerFactory` class to configure logging level granularity.

There are several log message sinks available,
including Stdout, DailyFile, and RotatingFile. Multiple sinks can be
configured, with independent logging levels. When adding a log sink
(described below), a shared pointer to the sink is returned. This
pointer allows you to set the log level using
`set_level(spdlog::level::level_enum level)`. The flush severity can
also be configured using flush_on (spdlog::level::level_enum level).
This setting will force the logger to flush its output when a log message of a certain level is received. Alternatively, the log level and
flush severity can be set globally using
`LoggerFactory::SetLoglevel(spdlog::level::level_enum level)` and
`LoggerFactory::SetFlushSeverity(spdlog::level::level_enum level)`.
These methods will overwrite any individual configurations.

#### Available Log Sinks

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

For a list of POPs where the API may be connected to
see [Points of Presence](POP.md).


## Common Problems

See [Common Problems](COMMON_PROBLEMS.md).
