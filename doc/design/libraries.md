# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libnullion_cli*         | RPC client functionality used by *nullion-cli* executable |
| *libnullion_common*      | Home for common functionality shared by different executables and libraries. Similar to *libnullion_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libnullion_consensus*   | Stable, backwards-compatible consensus functionality used by *libnullion_node* and *libnullion_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libnullionconsensus*    | Shared library build of static *libnullion_consensus* library |
| *libnullion_kernel*      | Consensus engine and support library used for validation by *libnullion_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libnullionqt*           | GUI functionality used by *nullion-qt* and *nullion-gui* executables |
| *libnullion_ipc*         | IPC functionality used by *nullion-node*, *nullion-wallet*, *nullion-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libnullion_node*        | P2P and RPC server functionality used by *nulliond* and *nullion-qt* executables. |
| *libnullion_util*        | Home for common functionality shared by different executables and libraries. Similar to *libnullion_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libnullion_wallet*      | Wallet functionality used by *nulliond* and *nullion-wallet* executables. |
| *libnullion_wallet_tool* | Lower-level wallet functionality used by *nullion-wallet* executable. |
| *libnullion_zmq*         | [ZeroMQ](../zmq.md) functionality used by *nulliond* and *nullion-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libnullion_consensus* and *libnullion_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libnullion_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libnullion_node* code lives in `src/node/` in the `node::` namespace
  - *libnullion_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libnullion_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libnullion_util* code lives in `src/util/` in the `util::` namespace
  - *libnullion_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

nullion-cli[nullion-cli]-->libnullion_cli;

nulliond[nulliond]-->libnullion_node;
nulliond[nulliond]-->libnullion_wallet;

nullion-qt[nullion-qt]-->libnullion_node;
nullion-qt[nullion-qt]-->libnullionqt;
nullion-qt[nullion-qt]-->libnullion_wallet;

nullion-wallet[nullion-wallet]-->libnullion_wallet;
nullion-wallet[nullion-wallet]-->libnullion_wallet_tool;

libnullion_cli-->libnullion_util;
libnullion_cli-->libnullion_common;

libnullion_common-->libnullion_consensus;
libnullion_common-->libnullion_util;

libnullion_kernel-->libnullion_consensus;
libnullion_kernel-->libnullion_util;

libnullion_node-->libnullion_consensus;
libnullion_node-->libnullion_kernel;
libnullion_node-->libnullion_common;
libnullion_node-->libnullion_util;

libnullionqt-->libnullion_common;
libnullionqt-->libnullion_util;

libnullion_wallet-->libnullion_common;
libnullion_wallet-->libnullion_util;

libnullion_wallet_tool-->libnullion_wallet;
libnullion_wallet_tool-->libnullion_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class nullion-qt,nulliond,nullion-cli,nullion-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libnullion_wallet* and *libnullion_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libnullion_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libnullion_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libnullion_common* should serve a similar function as *libnullion_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libnullion_util* and *libnullion_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for nullion-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libnullion_kernel* is not supposed to depend on *libnullion_common*, only *libnullion_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libnullion_kernel* should only depend on *libnullion_util* and *libnullion_consensus*.

- The only thing that should depend on *libnullion_kernel* internally should be *libnullion_node*. GUI and wallet libraries *libnullionqt* and *libnullion_wallet* in particular should not depend on *libnullion_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libnullion_consensus*, *libnullion_common*, and *libnullion_util*, instead of *libnullion_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libnullionqt*, *libnullion_node*, *libnullion_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libnullion_node* to *libnullion_kernel* as part of [The libnullionkernel Project #24303](https://github.com/nullion/nullion/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/nullion/nullion/issues/15732)
