<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>baseiids.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>baseiids_8cpp</filename>
    <namespace>Steinberg</namespace>
  </compound>
  <compound kind="file">
    <name>classfactoryhelpers.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>classfactoryhelpers_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>META_CREATE_FUNC</name>
      <anchorfile>classfactoryhelpers_8h.html</anchorfile>
      <anchor>afab65b35bec1c0222e99218f057b0a2c</anchor>
      <arglist>(funcName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLASS_CREATE_FUNC</name>
      <anchorfile>classfactoryhelpers_8h.html</anchorfile>
      <anchor>ae92c46dda6d8a5e8b0862006d1722484</anchor>
      <arglist>(className)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SINGLE_CREATE_FUNC</name>
      <anchorfile>classfactoryhelpers_8h.html</anchorfile>
      <anchor>a00b4a846a7ea809e79966dbb26cf86c0</anchor>
      <arglist>(className)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_META_CLASS</name>
      <anchorfile>classfactoryhelpers_8h.html</anchorfile>
      <anchor>a46dabd263537e5305c29e252e9d4e0df</anchor>
      <arglist>(className)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_META_CLASS_IFACE</name>
      <anchorfile>classfactoryhelpers_8h.html</anchorfile>
      <anchor>acc4233582b4e6b60c26daffd4e9d3573</anchor>
      <arglist>(className, Interface)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>META_CLASS</name>
      <anchorfile>classfactoryhelpers_8h.html</anchorfile>
      <anchor>af57be3698f87fcd9b59da1379350b19b</anchor>
      <arglist>(className)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>META_CLASS_IFACE</name>
      <anchorfile>classfactoryhelpers_8h.html</anchorfile>
      <anchor>a865cf36ad2027604dad50327bf9761b5</anchor>
      <arglist>(className, Interface)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>META_CLASS_SINGLE</name>
      <anchorfile>classfactoryhelpers_8h.html</anchorfile>
      <anchor>a63944ca72c7a04d6ec65301936202ffe</anchor>
      <arglist>(className, Interface)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>doc.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>doc_8h</filename>
  </compound>
  <compound kind="file">
    <name>fbuffer.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fbuffer_8cpp</filename>
    <includes id="fbuffer_8h" name="fbuffer.h" local="yes" imported="no">base/source/fbuffer.h</includes>
    <includes id="fstring_8h" name="fstring.h" local="yes" imported="no">base/source/fstring.h</includes>
    <namespace>Steinberg</namespace>
  </compound>
  <compound kind="file">
    <name>fbuffer.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fbuffer_8h</filename>
    <class kind="class">Steinberg::Buffer</class>
    <namespace>Steinberg</namespace>
  </compound>
  <compound kind="file">
    <name>fcleanup.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fcleanup_8h</filename>
    <class kind="struct">Steinberg::FDeleter</class>
    <class kind="struct">Steinberg::FArrayDeleter</class>
    <class kind="struct">Steinberg::FPtrNuller</class>
    <class kind="struct">Steinberg::FNuller</class>
    <class kind="struct">Steinberg::FBooleanSetter</class>
    <class kind="struct">Steinberg::FConditionalBoolSetter</class>
    <class kind="struct">Steinberg::FCloser</class>
    <class kind="class">Steinberg::FMallocReleaser</class>
    <namespace>Steinberg</namespace>
    <member kind="typedef">
      <type>FBooleanSetter&lt; bool &gt;</type>
      <name>FBoolSetter</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>aac248a0d0bf4446d45f2539bc61f6c23</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fcommandline.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fcommandline_8h</filename>
    <class kind="class">Steinberg::CommandLine::VariablesMap</class>
    <class kind="class">Steinberg::CommandLine::Description</class>
    <class kind="class">Steinberg::CommandLine::Descriptions</class>
    <namespace>Steinberg</namespace>
    <namespace>Steinberg::CommandLine</namespace>
    <member kind="typedef">
      <type>std::vector&lt; std::string &gt;</type>
      <name>FilesVector</name>
      <anchorfile>namespaceSteinberg_1_1CommandLine.html</anchorfile>
      <anchor>a8e4c7d85763c5d6cafd67588804308f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>parse</name>
      <anchorfile>namespaceSteinberg_1_1CommandLine.html</anchorfile>
      <anchor>aa59540066a0354da873325a175666974</anchor>
      <arglist>(int ac, char *av[], const Descriptions &amp;desc, VariablesMap &amp;result, FilesVector *files=nullptr)</arglist>
    </member>
    <member kind="function">
      <type>std::ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>namespaceSteinberg_1_1CommandLine.html</anchorfile>
      <anchor>a6a38965238a994bf87587aac0b83a5be</anchor>
      <arglist>(std::ostream &amp;os, const Descriptions &amp;desc)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fdebug.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fdebug_8cpp</filename>
    <includes id="fdebug_8h" name="fdebug.h" local="yes" imported="no">base/source/fdebug.h</includes>
    <member kind="function">
      <type>bool</type>
      <name>AmIBeingDebugged</name>
      <anchorfile>fdebug_8cpp.html</anchorfile>
      <anchor>a68905e29c7c08dd4acceb42b133e2cea</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isSmtgUnitTesting</name>
      <anchorfile>fdebug_8cpp.html</anchorfile>
      <anchor>a9644c7545455af00c4ed6d92818b4f11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSmtgUnitTesting</name>
      <anchorfile>fdebug_8cpp.html</anchorfile>
      <anchor>a52efd3b482fb6cae898073589f43d0c7</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fdebug.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fdebug_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_ASSERT</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>afe9433f0fc5b657a2f73863c413d8690</anchor>
      <arglist>(f)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_WARNING</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a5e5a76c97ca0b07d2c08ce8de068cef3</anchor>
      <arglist>(s)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_PRINTSYSERROR</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>aa58eec37e07b00b1ef4409319a6c1d48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_DEBUGSTR</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ac9bfa937bab4882709076060b7c00392</anchor>
      <arglist>(s)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_VERIFY</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ac9c56b9f1fc75b5e3738b1925418c949</anchor>
      <arglist>(f)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_VERIFY_IS</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a520468517905bd79f5d9948759592773</anchor>
      <arglist>(f, r)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_VERIFY_NOT</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a9fa41cf5abc4085415872f3ac6af6d5c</anchor>
      <arglist>(f, r)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_DBPRT0</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a81910a1b73bfa4f5ed5693dabb3ca055</anchor>
      <arglist>(a)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_DBPRT1</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>aa66fa2c41eb9ea81c7c91b4a7d0d1318</anchor>
      <arglist>(a, b)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_DBPRT2</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ab10a4ae99c89b67133e8a052d4d935aa</anchor>
      <arglist>(a, b, c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_DBPRT3</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ad4d5ecc14ee9d6837b2e6683db5e0921</anchor>
      <arglist>(a, b, c, d)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_DBPRT4</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>aa36f1d3aee0d987774c0b378c3f0d388</anchor>
      <arglist>(a, b, c, d, e)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_DBPRT5</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a1f6a3bf7072ece31220823a76e2841ae</anchor>
      <arglist>(a, b, c, d, e, f)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NEW</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ab6bca16ed021b1e211fde8669758f199</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NEWVEC</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>afb65a67a21fbcdc32ced37642a0ba690</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ASSERT</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>af343b20373ba49a92fce523e948f2ab3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WARNING</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a5cb439d9f933fde4cf23caa370c030e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUGSTR</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ad0a48883bb586ecd370427352cd3e017</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VERIFY</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ab64044259d5a2440725279dc7801f33e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VERIFY_IS</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a93c92efcb3b1c98a1ba51836017ebaad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VERIFY_NOT</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>accb3bda5a6288f9bf34cd6136c859473</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRINTSYSERROR</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ad48f9d61b00fa2397b11eb977c7785dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBPRT0</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a62bbf991e4c25adfb4e69bf873b0a5b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBPRT1</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>adf0a2e0c60ba7aee4aefe4f0b9a4cf32</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBPRT2</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>abe8e98ef330a8917afeea58f231c2fd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBPRT3</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ad35e7b53a5662120a18c70972419c62c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBPRT4</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a11a2c50ad704f0b137fd686f34331140</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBPRT5</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>ac57ffc0fbda8df0d6977b8df435a5d68</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>AmIBeingDebugged</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a68905e29c7c08dd4acceb42b133e2cea</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isSmtgUnitTesting</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a9644c7545455af00c4ed6d92818b4f11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSmtgUnitTesting</name>
      <anchorfile>fdebug_8h.html</anchorfile>
      <anchor>a52efd3b482fb6cae898073589f43d0c7</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fdynlib.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fdynlib_8cpp</filename>
    <includes id="fdynlib_8h" name="fdynlib.h" local="yes" imported="no">base/source/fdynlib.h</includes>
    <includes id="fstring_8h" name="fstring.h" local="yes" imported="no">base/source/fstring.h</includes>
    <namespace>Steinberg</namespace>
  </compound>
  <compound kind="file">
    <name>fdynlib.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fdynlib_8h</filename>
    <includes id="fobject_8h" name="fobject.h" local="yes" imported="no">base/source/fobject.h</includes>
    <class kind="class">Steinberg::FDynLibrary</class>
    <namespace>Steinberg</namespace>
  </compound>
  <compound kind="file">
    <name>fobject.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fobject_8cpp</filename>
    <includes id="fobject_8h" name="fobject.h" local="yes" imported="no">base/source/fobject.h</includes>
    <namespace>Steinberg</namespace>
    <namespace>Steinberg::Singleton</namespace>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_VALIDATE_DEPENDENCY_COUNT</name>
      <anchorfile>fobject_8cpp.html</anchorfile>
      <anchor>a9550d93de76bc0422a328bc17dfaa0c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::vector&lt; FObject ** &gt;</type>
      <name>ObjectVector</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ae71bce85324f37d598620c2adc38733f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isTerminated</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a84acfabb69fd8f78cc6afad4301357b7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lockRegister</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a1612cf2b7b00489ee4cd0af7999ff7ac</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unlockRegister</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a2aad80bf0469cb9153a7666f2b2b4817</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>registerInstance</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ad6e6c857d3abf9d8ef37ef8d465c9977</anchor>
      <arglist>(FObject **o)</arglist>
    </member>
    <member kind="variable">
      <type>struct Steinberg::FObjectIIDInitializer</type>
      <name>gFObjectIidInitializer</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a716e8faa9615ce0d3a3b357275d14868</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ObjectVector *</type>
      <name>singletonInstances</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ab5cb5c38a31d778cf922b01947589311</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>singletonsTerminated</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ae52f18a708ab305abe54c48ffc1c9840</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Steinberg::Base::Thread::FLock *</type>
      <name>singletonsLock</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ac67df8722321fe3f07d688a9710006cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct Steinberg::Singleton::Deleter</type>
      <name>deleter</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ad6199b16f89340fdc59b661207d8ae40</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fobject.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fobject_8h</filename>
    <includes id="fdebug_8h" name="fdebug.h" local="yes" imported="no">base/source/fdebug.h</includes>
    <class kind="class">Steinberg::FObject</class>
    <namespace>Steinberg</namespace>
    <namespace>Steinberg::Singleton</namespace>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_DEPENDENCY_COUNT</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>acc5dd3332b6e78cc206fe66cdd96f347</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SINGLETON</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a69f2b09f8941011cb853819558eff4a7</anchor>
      <arglist>(ClassName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OBJ_METHODS</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a5a6e2365f969f95aee286638cf6164c7</anchor>
      <arglist>(className, baseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REFCOUNT_METHODS</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>aebbb3846780fdaf18f8ee89b73ff149d</anchor>
      <arglist>(BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_INTERFACES</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a3c0ce0ce836f44d269b414624e3e0971</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACE</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a53e7d200032b573e38e925748fdb89e3</anchor>
      <arglist>(InterfaceName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>END_DEFINE_INTERFACES</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a7991876302dbc825f493930c856dbd83</anchor>
      <arglist>(BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACES_1</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>aba57dd99d0ef259ecc04fc3d05734e88</anchor>
      <arglist>(InterfaceName, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACES_2</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a334b586ec792ec3bcd90863cc61eb27c</anchor>
      <arglist>(InterfaceName1, InterfaceName2, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACES_3</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a06f8f8494473247c3a89e0627600b0ad</anchor>
      <arglist>(InterfaceName1, InterfaceName2, InterfaceName3, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACES_4</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a8fa73969db52bf1e78aaea1b2175128e</anchor>
      <arglist>(InterfaceName1, InterfaceName2, InterfaceName3, InterfaceName4, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FUNKNOWN_METHODS</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a1ad212b9357c288b64de6b6a46c19a57</anchor>
      <arglist>(InterfaceName, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FUNKNOWN_METHODS2</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>ad07f904465df9375353c5ca367a5dfb2</anchor>
      <arglist>(InterfaceName1, InterfaceName2, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FUNKNOWN_METHODS3</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>aa888a63aad080dec3651edbb764cd8bd</anchor>
      <arglist>(InterfaceName1, InterfaceName2, InterfaceName3, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FUNKNOWN_METHODS4</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a41a8ea197ab9208b554473144f6572fd</anchor>
      <arglist>(InterfaceName1, InterfaceName2, InterfaceName3, InterfaceName4, BaseClass)</arglist>
    </member>
    <member kind="typedef">
      <type>FIDString</type>
      <name>FClassID</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab36f267af0b3dc88ce9135a0c3b4f170</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>C *</type>
      <name>FCast</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7eedfe7a9261c91c5c4725c762d0302d</anchor>
      <arglist>(const FObject *object)</arglist>
    </member>
    <member kind="function">
      <type>C *</type>
      <name>FCast</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a64e77e379512698cdbed2c00f9a6dd38</anchor>
      <arglist>(FUnknown *unknown)</arglist>
    </member>
    <member kind="function">
      <type>C *</type>
      <name>FUCast</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a0f07be811035c79903a4239709c274f3</anchor>
      <arglist>(FObject *object)</arglist>
    </member>
    <member kind="function">
      <type>C *</type>
      <name>FUCast</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>aec0fb92a1a224a4dc1a4e323505e52d5</anchor>
      <arglist>(FUnknown *object)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>AssignShared</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a061c8d1b219f1b80adf65d2c1d36e899</anchor>
      <arglist>(T *&amp;dest, T *newPtr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>AssignSharedDependent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a52db63df2d44c7a493bf7b5ce39df1a6</anchor>
      <arglist>(IDependent *_this, T *&amp;dest, T *newPtr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>AssignSharedDependent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a8567982a150047c011e89c7185871d96</anchor>
      <arglist>(IDependent *_this, IPtr&lt; T &gt; &amp;dest, T *newPtr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeReleaseDependent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a0869140d4f11c3f1dae20794daa85b5f</anchor>
      <arglist>(IDependent *_this, T *&amp;dest)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeReleaseDependent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ad047921ea5c9ffd8f4b5e2c31d30d819</anchor>
      <arglist>(IDependent *_this, IPtr&lt; T &gt; &amp;dest)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>registerInstance</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ad6e6c857d3abf9d8ef37ef8d465c9977</anchor>
      <arglist>(FObject **o)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isTerminated</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a84acfabb69fd8f78cc6afad4301357b7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lockRegister</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a1612cf2b7b00489ee4cd0af7999ff7ac</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unlockRegister</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a2aad80bf0469cb9153a7666f2b2b4817</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeRelease</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a694fec3dd0db8909ea3289ba1de4f494</anchor>
      <arglist>(I *&amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeRelease</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a2b4982585b3f7bc20622a0f5b81fcb43</anchor>
      <arglist>(IPtr&lt; I &gt; &amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeDelete</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a1a7b3d6a54e6b821d843acdbb90ec7a1</anchor>
      <arglist>(T *&amp;ptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_INTERFACES</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a3c0ce0ce836f44d269b414624e3e0971</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACE</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a53e7d200032b573e38e925748fdb89e3</anchor>
      <arglist>(InterfaceName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>END_DEFINE_INTERFACES</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a7991876302dbc825f493930c856dbd83</anchor>
      <arglist>(BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACES_1</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>aba57dd99d0ef259ecc04fc3d05734e88</anchor>
      <arglist>(InterfaceName, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACES_2</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a334b586ec792ec3bcd90863cc61eb27c</anchor>
      <arglist>(InterfaceName1, InterfaceName2, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACES_3</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a06f8f8494473247c3a89e0627600b0ad</anchor>
      <arglist>(InterfaceName1, InterfaceName2, InterfaceName3, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_INTERFACES_4</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a8fa73969db52bf1e78aaea1b2175128e</anchor>
      <arglist>(InterfaceName1, InterfaceName2, InterfaceName3, InterfaceName4, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FUNKNOWN_METHODS</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a1ad212b9357c288b64de6b6a46c19a57</anchor>
      <arglist>(InterfaceName, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FUNKNOWN_METHODS2</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>ad07f904465df9375353c5ca367a5dfb2</anchor>
      <arglist>(InterfaceName1, InterfaceName2, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FUNKNOWN_METHODS3</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>aa888a63aad080dec3651edbb764cd8bd</anchor>
      <arglist>(InterfaceName1, InterfaceName2, InterfaceName3, BaseClass)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FUNKNOWN_METHODS4</name>
      <anchorfile>fobject_8h.html</anchorfile>
      <anchor>a41a8ea197ab9208b554473144f6572fd</anchor>
      <arglist>(InterfaceName1, InterfaceName2, InterfaceName3, InterfaceName4, BaseClass)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeRelease</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a694fec3dd0db8909ea3289ba1de4f494</anchor>
      <arglist>(I *&amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeRelease</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a2b4982585b3f7bc20622a0f5b81fcb43</anchor>
      <arglist>(IPtr&lt; I &gt; &amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeDelete</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a1a7b3d6a54e6b821d843acdbb90ec7a1</anchor>
      <arglist>(T *&amp;ptr)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fstdmethods.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fstdmethods_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_VARIABLE</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>ac7b33405a809656816db90c2e08874fc</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_POINTER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a1ca15419dc50b5ec2cc22adad87ed510</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a7bdc96c0dcfb2b23658d8f985a83048b</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COMPARE_BY_MEMBER_METHODS</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>aca2d3c80e58016d970e75479136fe8bf</anchor>
      <arglist>(className, memberName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COMPARE_BY_MEMORY_METHODS</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a236e4ee9734fadabedc1f4d6e877e2ab</anchor>
      <arglist>(className)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COMPARE_BY_COMPARE_METHOD</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>ad013585d1ece4615f652ec6bf5929cef</anchor>
      <arglist>(className, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_STATE</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a3aa7211bde6c894d83fb000e32667c45</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_GETSTATE</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a8eaa236016023ebe7fc553a03529d4b9</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_FLAG</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>ad2fa6d3fa7d143009198b5b4d744c2dc</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_GETFLAG</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a8095ab68981a9e2ab6e471a6f1b515f6</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_FLAG_STATIC</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a5d0a2bb91ac8f0fc96153c610f872a92</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DATA_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a796d97a5cd47b9a00411cf71ab6ebf71</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLASS_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a8bc055fe95609ed757fbc5f03b5c28c1</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>POINTER_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a894c0fe14fa8fad008264abe4a3660b1</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SHARED_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a9fe746aafac5178bf27bf6ac3fcaca4c</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OWNED_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a5c19e8bb326e36e0ffd0ec5447c38dd4</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRING_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a0efb389fce03cc1bc8190565a8adaa0a</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRING8_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a7f1231ffc827e3c4244a9b30c43ed5e2</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRING_MEMBER_STD</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a19e11906d2147132f158e03e253398a1</anchor>
      <arglist>(varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRING8_MEMBER_STD</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>ac846c65c8e67ae789595f6dd95cba54c</anchor>
      <arglist>(varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_STATE</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a3aa7211bde6c894d83fb000e32667c45</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_GETSTATE</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a8eaa236016023ebe7fc553a03529d4b9</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_FLAG</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>ad2fa6d3fa7d143009198b5b4d744c2dc</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_GETFLAG</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a8095ab68981a9e2ab6e471a6f1b515f6</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFINE_FLAG_STATIC</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a5d0a2bb91ac8f0fc96153c610f872a92</anchor>
      <arglist>(flagVar, methodName, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DATA_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a796d97a5cd47b9a00411cf71ab6ebf71</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLASS_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a8bc055fe95609ed757fbc5f03b5c28c1</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>POINTER_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a894c0fe14fa8fad008264abe4a3660b1</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SHARED_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a9fe746aafac5178bf27bf6ac3fcaca4c</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OWNED_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a5c19e8bb326e36e0ffd0ec5447c38dd4</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRING_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a0efb389fce03cc1bc8190565a8adaa0a</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRING8_MEMBER</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a7f1231ffc827e3c4244a9b30c43ed5e2</anchor>
      <arglist>(type, varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRING_MEMBER_STD</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>a19e11906d2147132f158e03e253398a1</anchor>
      <arglist>(varName, methodName)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRING8_MEMBER_STD</name>
      <anchorfile>fstdmethods_8h.html</anchorfile>
      <anchor>ac846c65c8e67ae789595f6dd95cba54c</anchor>
      <arglist>(varName, methodName)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fstreamer.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fstreamer_8cpp</filename>
    <includes id="fstreamer_8h" name="fstreamer.h" local="yes" imported="no">fstreamer.h</includes>
    <includes id="fstring_8h" name="fstring.h" local="yes" imported="no">base/source/fstring.h</includes>
    <includes id="fbuffer_8h" name="fbuffer.h" local="yes" imported="no">base/source/fbuffer.h</includes>
    <namespace>Steinberg</namespace>
  </compound>
  <compound kind="file">
    <name>fstreamer.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fstreamer_8h</filename>
    <class kind="class">Steinberg::FStreamer</class>
    <class kind="class">Steinberg::FStreamSizeHolder</class>
    <class kind="class">Steinberg::IBStreamer</class>
    <namespace>Steinberg</namespace>
    <member kind="enumeration">
      <type></type>
      <name>FSeekMode</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a78e5e10c2df97a6a5a6096069ade24db</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSeekSet</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a78e5e10c2df97a6a5a6096069ade24dbad9fe3df36943c836f26fd55f2e800384</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSeekCurrent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a78e5e10c2df97a6a5a6096069ade24dbaff1a9d5b4238462a49619084e8324b85</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSeekEnd</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a78e5e10c2df97a6a5a6096069ade24dbaa2d1805ccbf869dc4a610f9571d83d7f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fstring.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fstring_8cpp</filename>
    <includes id="fstring_8h" name="fstring.h" local="yes" imported="no">base/source/fstring.h</includes>
    <includes id="fdebug_8h" name="fdebug.h" local="yes" imported="no">base/source/fdebug.h</includes>
    <namespace>Steinberg</namespace>
    <member kind="define">
      <type>#define</type>
      <name>kPrintfBufferSize</name>
      <anchorfile>fstring_8cpp.html</anchorfile>
      <anchor>aa62de8c8355a305d6e9095fbff0c7c9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_STRING_CHECK_CONVERSION</name>
      <anchorfile>fstring_8cpp.html</anchorfile>
      <anchor>a4d8936bba50eec59bad21b4d3ba3ccd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_STRING_CHECK_CONVERSION_NO_BREAK</name>
      <anchorfile>fstring_8cpp.html</anchorfile>
      <anchor>ae9b310832caa4fdd9b19f123b86e8cab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMTG_STRING_CHECK_MSG</name>
      <anchorfile>fstring_8cpp.html</anchorfile>
      <anchor>a31965eb0cfa7cfcf80410f2b94c40086</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::codecvt_utf8_utf16&lt; char16_t &gt;</type>
      <name>ConverterFacet</name>
      <anchorfile>fstring_8cpp.html</anchorfile>
      <anchor>a1ece0eb061ed782654f2aadfd088e204</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::wstring_convert&lt; ConverterFacet, char16_t &gt;</type>
      <name>Converter</name>
      <anchorfile>fstring_8cpp.html</anchorfile>
      <anchor>a243f56b639b9bb0d1850454b40ea0cb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hashString8</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a166ebc6677e19253cda1f55ead2b8106</anchor>
      <arglist>(const char8 *s, uint32 m)</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hashString16</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a61e26dc8165fa82a90f930a1f8367a3c</anchor>
      <arglist>(const char16 *s, uint32 m)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>tstrnatcmp</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>adcccbbf0a6580038c4fb1f0dbf91667f</anchor>
      <arglist>(const T *s1, const T *s2, bool caseSensitive=true)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>strnatcmp8</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab16cda4272acda12af7fe328eb3abbfa</anchor>
      <arglist>(const char8 *s1, const char8 *s2, bool caseSensitive)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>strnatcmp16</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a026eef900ab2e9f3c1390d73992a48a2</anchor>
      <arglist>(const char16 *s1, const char16 *s2, bool caseSensitive)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fstring.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>fstring_8h</filename>
    <includes id="fobject_8h" name="fobject.h" local="yes" imported="no">base/source/fobject.h</includes>
    <class kind="class">Steinberg::ConstString</class>
    <class kind="class">Steinberg::String</class>
    <class kind="class">Steinberg::StringObject</class>
    <namespace>Steinberg</namespace>
    <member kind="enumeration">
      <type></type>
      <name>MBCodePage</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_ANSI</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198a4bc87ed163042e19a3c8c857f320e601</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_MAC_ROMAN</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198aece485d20e1f11025207f5d03331170b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_ANSI_WEL</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198ae306671b758491ebea5651afc41faf06</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_MAC_CEE</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198afdfb28513dca7897ba46509177f3f295</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_Utf8</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198a42fe778326afe1a478de66fa0ae88053</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_ShiftJIS</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198a800fe314d21309f85659ee3c8ff41fba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_US_ASCII</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198a68f73e16ffd280be8ea415937c17901e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_Default</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198ab93a845dd7dd03d5e296ffff03a78aa2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>UnicodeNormalization</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kUnicodeNormC</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5a5ab93cdbd8bb16cc7261b7d02f656426</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kUnicodeNormD</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5a719813623d8e0a706e6ab6cd2a0fb0f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kUnicodeNormKC</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5a6bc71341f3a88c030bc7f46992d39c99</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kUnicodeNormKD</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5ad8daa621694a792a6b4368b2e1caccad</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hashString8</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a166ebc6677e19253cda1f55ead2b8106</anchor>
      <arglist>(const char8 *s, uint32 m)</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hashString16</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a61e26dc8165fa82a90f930a1f8367a3c</anchor>
      <arglist>(const char16 *s, uint32 m)</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hashString</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab9e1f71a717bf28978e67938cf197360</anchor>
      <arglist>(const char *s, uint32 m)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ada25fd2a2681fec14af4507826ffdfe8</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a2c3adccbf0361ed1e35dc58f474aa783</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a0047a2ab78ce4594023b5691eda91128</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>adafb26f233ad745fb7503011697a8d89</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a4d79863da188e183ae4150116906ad6b</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a1a02a54515930b11df9cb7c05ed0a663</anchor>
      <arglist>(const ConstString &amp;s1, const String &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af032c74c6993e1abfbd7bccda3fea33d</anchor>
      <arglist>(const String &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ac18cc72253d13ec535ba0187d07759af</anchor>
      <arglist>(const String &amp;s1, const String &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>aa19edede938efc8875df07e2079cb898</anchor>
      <arglist>(const String &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ae356517d1995baa9053ae4cbec848b85</anchor>
      <arglist>(const String &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a86f7c6c4e0f84a11e5583452e809debe</anchor>
      <arglist>(const char8 *s1, const String &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a484a5c7b2980c3eddde1777f4f8740e2</anchor>
      <arglist>(const char16 *s1, const String &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ad5ce23c4f9cca43a9f66ef23d9325159</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>afc04c8ac4717595b641a119f8e1f4c08</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ac98deb5a1f36329c44edeee647d46431</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a98082cddf36f42dae42928aab7b7d704</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af65b89110f91b435a658a09cde45ee6b</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>aaad28d7939d8454ba62e2ed43e6b6a07</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>acf5c2d96088a26678de1f7ba2b0610a9</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab4e3d70dc9e542aa4830d9026f49e318</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a296a0841f7c8f2fc3a5708556c720a5a</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a111fe3210982295b10c2b0a6c64f8849</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a8e6d100fac4f2e9011a595eb516d21ea</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>acf534ee8bc3f7368a3a21e666eb87e88</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7e81e3384f1e73b68dba43f8ca561b27</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7784e17f867a990478e8b3a25216f047</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7792b96abbdb52f88644a2d7fdc0635d</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a763dcfcbf62f158517e3265575e09b03</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af532a1fe0c1fa0b787fb1c6fa58048e4</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a294195888b30e4aa51ac89171313dddd</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7a3ef53c29d18481261d4ad40fbd93d8</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a5dac4595f7ec9120e736fc4a72b263a4</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a59823787882c26e6dc2a849a23785b22</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a1bfad3a77225793f845ad097c4b694a6</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a5998cbea9b1218b35018328826e6a40d</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a34e7201686726b43099edeabc6e64b02</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a4a5e176ffe4e3d42b9dbdd5ff6b6a232</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab43ab059cf7a1b2eee6dcf7db9429f72</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af8868f64b08e62fc4ca89095b748dcb3</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a70fa44d88af75be3d8d5402aa761370c</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a645271d1621265af41557ff516b67961</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a262d596d26d40f41fab24af7162c7020</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>strnatcmp8</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab16cda4272acda12af7fe328eb3abbfa</anchor>
      <arglist>(const char8 *s1, const char8 *s2, bool caseSensitive)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>strnatcmp16</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a026eef900ab2e9f3c1390d73992a48a2</anchor>
      <arglist>(const char16 *s1, const char16 *s2, bool caseSensitive)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>strnatcmp</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af813b691586a7d0180f9275b88d8ce86</anchor>
      <arglist>(const char *s1, const char *s2, bool caseSensitive=true)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>hexbinary.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>hexbinary_8h</filename>
    <includes id="fbuffer_8h" name="fbuffer.h" local="yes" imported="no">base/source/fbuffer.h</includes>
    <namespace>Steinberg</namespace>
    <namespace>Steinberg::HexBinary</namespace>
    <member kind="function">
      <type>bool</type>
      <name>decode</name>
      <anchorfile>namespaceSteinberg_1_1HexBinary.html</anchorfile>
      <anchor>a567d62615b1d9ccab3b175b4b9fc61f0</anchor>
      <arglist>(const void *input, int32 inputSize, Buffer &amp;result)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>encode</name>
      <anchorfile>namespaceSteinberg_1_1HexBinary.html</anchorfile>
      <anchor>a409cabd0268ae39045fe68f7816a06af</anchor>
      <arglist>(const void *input, int32 inputSize, Buffer &amp;result)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>timer.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>timer_8cpp</filename>
    <includes id="timer_8h" name="timer.h" local="yes" imported="no">base/source/timer.h</includes>
    <namespace>Steinberg</namespace>
    <namespace>Steinberg::SystemTime</namespace>
    <member kind="function">
      <type>int32</type>
      <name>getTicks</name>
      <anchorfile>namespaceSteinberg_1_1SystemTime.html</anchorfile>
      <anchor>ae994ceba61021313d4200996d46787f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint64</type>
      <name>getTicks64</name>
      <anchorfile>namespaceSteinberg_1_1SystemTime.html</anchorfile>
      <anchor>accc64954e88f0a32add172d0189918dc</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>timer.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>timer_8h</filename>
    <includes id="fobject_8h" name="fobject.h" local="yes" imported="no">fobject.h</includes>
    <class kind="class">Steinberg::ITimerCallback</class>
    <class kind="class">Steinberg::Timer</class>
    <class kind="class">Steinberg::DisableDispatchingTimers</class>
    <namespace>Steinberg</namespace>
    <namespace>Steinberg::SystemTime</namespace>
    <member kind="function">
      <type>uint64</type>
      <name>getTicks64</name>
      <anchorfile>namespaceSteinberg_1_1SystemTime.html</anchorfile>
      <anchor>accc64954e88f0a32add172d0189918dc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint64</type>
      <name>getTicksDuration</name>
      <anchorfile>namespaceSteinberg_1_1SystemTime.html</anchorfile>
      <anchor>a5eb6e497830d117c9591a9d18faa3816</anchor>
      <arglist>(uint64 old, uint64 now)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>getTicks</name>
      <anchorfile>namespaceSteinberg_1_1SystemTime.html</anchorfile>
      <anchor>ae994ceba61021313d4200996d46787f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ITimerCallback *</type>
      <name>newTimerCallback</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af754042087ab79da3c3b0d08681e9a93</anchor>
      <arglist>(const Call &amp;call)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>updatehandler.cpp</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>updatehandler_8cpp</filename>
    <includes id="updatehandler_8h" name="updatehandler.h" local="yes" imported="no">base/source/updatehandler.h</includes>
    <includes id="classfactoryhelpers_8h" name="classfactoryhelpers.h" local="yes" imported="no">base/source/classfactoryhelpers.h</includes>
    <includes id="fstring_8h" name="fstring.h" local="yes" imported="no">base/source/fstring.h</includes>
    <namespace>Steinberg</namespace>
    <member kind="define">
      <type>#define</type>
      <name>NON_EXISTING_DEPENDENCY_CHECK</name>
      <anchorfile>updatehandler_8cpp.html</anchorfile>
      <anchor>a71de32ab81686f17de125be381adefd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLASS_NAME_TRACKED</name>
      <anchorfile>updatehandler_8cpp.html</anchorfile>
      <anchor>a748343c42faddaaa8b263d379c45a1a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DEF_CLASS_IID</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a03374ef5cb2e4ca5d36313f4c3dd85e2</anchor>
      <arglist>(IUpdateManager) namespace Update</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>updatehandler.h</name>
    <path>/Users/buildserver/Builddata/re/295942927/b/VST_SDK/vst3sdk/base/source/</path>
    <filename>updatehandler_8h</filename>
    <includes id="fobject_8h" name="fobject.h" local="yes" imported="no">base/source/fobject.h</includes>
    <class kind="class">Steinberg::IUpdateManager</class>
    <class kind="class">Steinberg::UpdateHandler</class>
    <namespace>Steinberg</namespace>
  </compound>
  <compound kind="class">
    <name>Steinberg::Buffer</name>
    <filename>classSteinberg_1_1Buffer.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>swapSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a24e80486061f2d500092ee60eb3a21a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSwap16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a24e80486061f2d500092ee60eb3a21a5a0de688fd6dd63aec086c2004e20cdced</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSwap32</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a24e80486061f2d500092ee60eb3a21a5a89f97cdfea8dbe86ba7b2f6a8e1f73d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSwap64</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a24e80486061f2d500092ee60eb3a21a5a6df3cbd095d4dacb56f3fa2095382edc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSwap16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a24e80486061f2d500092ee60eb3a21a5a0de688fd6dd63aec086c2004e20cdced</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSwap32</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a24e80486061f2d500092ee60eb3a21a5a89f97cdfea8dbe86ba7b2f6a8e1f73d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSwap64</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a24e80486061f2d500092ee60eb3a21a5a6df3cbd095d4dacb56f3fa2095382edc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Buffer</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a47587d18a394fb4b9508c61e3d5f0260</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Buffer</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>abfbc60030ca169331f33b68b052677c7</anchor>
      <arglist>(const void *b, uint32 size)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Buffer</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a7d37d8ae2357c047daeba8de0cbe7425</anchor>
      <arglist>(uint32 size, uint8 initVal)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Buffer</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a9ceade440770339ca2768ff71ad8927f</anchor>
      <arglist>(uint32 size)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Buffer</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ac0d4f265450e68c7cd8e4df9a6a7150c</anchor>
      <arglist>(const Buffer &amp;buff)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Buffer</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ac72b418d40e29411ac044a49a20be750</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>af108b63e6b6c4d37ad7ca8aa38f91722</anchor>
      <arglist>(const Buffer &amp;buff)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>acf2b725d02e8aac1078fdd12d9548d9a</anchor>
      <arglist>(const Buffer &amp;buff) const</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>getSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad9d472f21a328bddfd727cabaca72bac</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a5db5816376f3e60cbe277ed6f714f5b4</anchor>
      <arglist>(uint32 newSize)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>grow</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a61fc1961d3b66e3bdb23cc8da86872d4</anchor>
      <arglist>(uint32 memSize)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setMaxSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>adbdb3ea09c55b9e12675c45709999dfe</anchor>
      <arglist>(uint32 size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>fillup</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ab4c01847d01bd569b8f00a95dd852968</anchor>
      <arglist>(uint8 initVal=0)</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>getFillSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>aa46ef048f207654b8d2dc8252b838298</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setFillSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>aeb44fb76e1d019848f6e3fae79232254</anchor>
      <arglist>(uint32 c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>flush</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>adac116554b543b7c4228c018a85882f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>truncateToFillSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a1a3b64cb526cc540ab6e7e533ab07efd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isFull</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad753310e7ebeac80873cea6766ad1e60</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>getFree</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a4d09ff38eea691c31b3f0cff2025906e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>shiftStart</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a79b7870538d70d8e72427d8cb1504917</anchor>
      <arglist>(int32 amount)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>shiftAt</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ac3d0af936a5a4f5f9aa26b378e9f3ff5</anchor>
      <arglist>(uint32 position, int32 amount)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>move</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a0a728693e22178550b0bda95b139e684</anchor>
      <arglist>(int32 amount, uint8 initVal=0)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>copy</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a9ee54180cb05034c130c3328c2617c85</anchor>
      <arglist>(uint32 from, uint32 to, uint32 bytes)</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>get</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad9f85b2a73d3c78663a595b3790a268c</anchor>
      <arglist>(void *b, uint32 size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDelta</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a1331a84eeb64cb0ab5a92826a3f61f7a</anchor>
      <arglist>(uint32 d)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a9cf73b2a1331acbbe6b1be8539e4236f</anchor>
      <arglist>(uint8)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a5afd335e2217f540340d3992b77976d0</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a244c4ccf1bce32c9d8c58565e99ea7a7</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad31fbc079d7d6d528336744b24c5329e</anchor>
      <arglist>(const void *, uint32 size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a1ddf189b20aeaf992c269b92f2178924</anchor>
      <arglist>(void *, uint32 size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ab3ff373695d7d49be1a96c7991251708</anchor>
      <arglist>(uint8 *, uint32 size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>afa5babad6752cc4ab2f3a537cff90143</anchor>
      <arglist>(char8 *, uint32 size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>abd2c137bca66738bab879a130915f350</anchor>
      <arglist>(const uint8 *, uint32 size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad11514107292a2033f0ac67ceb3d06ed</anchor>
      <arglist>(const char8 *, uint32 size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>put</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a92aee5c5993e158144843f5adea15213</anchor>
      <arglist>(const String &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ac51e247714f6833a9a56ae181cf9bc23</anchor>
      <arglist>(uint8 value)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>aa7103c38867a7a83a2de3f8d32a0017c</anchor>
      <arglist>(const char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a474a14913e4b4c828cd03526c8424a9e</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>af3a74022bfba9a0d1d72edd374e3aa64</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ab75e024e20c6803e223d1901081e8ad7</anchor>
      <arglist>(const char8 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a0c371b54df60e3f5f952a344c99ae8e9</anchor>
      <arglist>(const char16 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad6b171f5e02e782a693c78058242ac12</anchor>
      <arglist>(char8 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ada178b76cfdca76e3f5f2e72e0d75e80</anchor>
      <arglist>(unsigned char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>aea650e455e820edeeefcca21e3e0dc30</anchor>
      <arglist>(const unsigned char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a00b9566f11157ea3cdd2a60763106813</anchor>
      <arglist>(char8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a432a94deb1ef653480c234596372d29a</anchor>
      <arglist>(unsigned char c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a869841b0e5c51a1fad8b6d7b8255a023</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>appendString16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a0d0c8edd6a4712962cb5a114fb70a754</anchor>
      <arglist>(char16 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a5a6c749087e39a155dbfa68142e62231</anchor>
      <arglist>(const char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a175a4813b165d61a9dd7362d242f67b5</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a0a300c804244e59c525ea96c0aee80e1</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a32a98866cea235d36c99b7ab68fe1368</anchor>
      <arglist>(const char8 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a0b084cf73146b68fd85d411cafe02465</anchor>
      <arglist>(const char16 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a5a30b522f8105c19e3748badecc5c1e0</anchor>
      <arglist>(char8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a5e3b2d2909e45c472d86390a1ced9337</anchor>
      <arglist>(unsigned char c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a8ae0a64a6ea715f157f6612d339253a7</anchor>
      <arglist>(char8 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad7a76bee549fe98d9de67a5cf40e43b9</anchor>
      <arglist>(unsigned char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a25229e8dc072c6fd12fe2f842f25a920</anchor>
      <arglist>(const unsigned char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a6b4fe835e9a14bb7f1f2e67ca3e6ba7b</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>prependString16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a4daa5336dd993fca07ba45b6dae92a6e</anchor>
      <arglist>(char16 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ae2b6ce9c093e49b39fceab9f8f546874</anchor>
      <arglist>(const char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>aca9260d196dd4ec64a43fb143b5f1023</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a0e6e550f673e57e20d13687c4899e7e4</anchor>
      <arglist>(const char16 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ac4a1150f070f27758448a37b44cd4906</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>af720d2e7b68280f0021a2d0bb60e0bcb</anchor>
      <arglist>(const char *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a242d18cd2364dd753e6759639f5f02fb</anchor>
      <arglist>(const char16 *s)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad4f0dac69471a52927cf9a2f0e75b438</anchor>
      <arglist>(char8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a8782a69f0de3929147f537cfc25d3873</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>endString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a50de18138bdd773dfc112c841c8597a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>endString8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a4ddfa41c4e0f46580c84664c30f46fca</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>endString16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ad487b6b6fedb84790a3820aa27b407ad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>makeHexString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a22c9694174100cb1feff3900a2b826cf</anchor>
      <arglist>(String &amp;result)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>fromHexString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a47d47e5af0c48f358ca67d09148c572c</anchor>
      <arglist>(const char8 *string)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator void *</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ac34a13938612201ccf01b9d70b1eeae7</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>str</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a75f1a295bdb596f1244e5ec8210159b2</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>char8 *</type>
      <name>str8</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a5c9c7409f3e36305573237e15c77f6d1</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>char16 *</type>
      <name>str16</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a380beec1d895068fe2e12b42f437b60c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int8 *</type>
      <name>int8Ptr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a9783abfc1b86c4ada9e313a4e7d85c17</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>uint8 *</type>
      <name>uint8Ptr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a8c7c2e1b7f43b29736efc89421a528c9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int16 *</type>
      <name>int16Ptr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a292971e6d6915cd6911245b8bcc896d0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>uint16 *</type>
      <name>uint16Ptr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a3f51ffe69617d4c02270c3c79c7c6231</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int32 *</type>
      <name>int32Ptr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a59477c046f444c31d514150d983760cc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>uint32 *</type>
      <name>uint32Ptr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a59be8033be1f4c53fc98786cc8d7011d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>float *</type>
      <name>floatPtr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a912d385531902483c194cf3196193b0a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double *</type>
      <name>doublePtr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a12ce1b09d51f9e84b79a427e8bb3d1c2</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>char16 *</type>
      <name>wcharPtr</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ae13bd82bffbf058fb8a58a86ec3b57dc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int8 *</type>
      <name>operator+</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>ab1f9c3c721c22713ec76cdb702c25a85</anchor>
      <arglist>(uint32 i)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>operator !</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a36616915c7fe012a5fdab71c5e0cef8c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>swap</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a78970931ab3488ad189d421140692228</anchor>
      <arglist>(int16 swapSize)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>take</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>acfa5f874d6e079d5dd3ff1a19746cb03</anchor>
      <arglist>(Buffer &amp;from)</arglist>
    </member>
    <member kind="function">
      <type>int8 *</type>
      <name>pass</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>afa32c58b633cbb67448023400d78b11d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>toWideString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a74cec25c301d7fc9037c2831909325aa</anchor>
      <arglist>(int32 sourceCodePage)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>toMultibyteString</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>abc435345b9e229e7dbfc09c51854890d</anchor>
      <arglist>(int32 destCodePage)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>swap</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a8499ff64797ee5f037de8529841fdee2</anchor>
      <arglist>(void *buffer, uint32 bufferSize, int16 swapSize)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int8 *</type>
      <name>buffer</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>afef95cf6c683b6331e76f83606c53d55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>uint32</type>
      <name>memSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>aafd1ae1da95950e08d65aa4025690aff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>uint32</type>
      <name>fillSize</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>abf40bac133b6bd7a1cb243fe235f0f96</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>uint32</type>
      <name>delta</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a51785d72370a4b435e5863a25705f6c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static const uint32</type>
      <name>defaultDelta</name>
      <anchorfile>classSteinberg_1_1Buffer.html</anchorfile>
      <anchor>a5ddbcd843e127d9a3b72dd34642eccb4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::ConstString</name>
    <filename>classSteinberg_1_1ConstString.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>CompareMode</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af4045f11e8887dda083d0122d24df4f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCaseSensitive</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af4045f11e8887dda083d0122d24df4f6a911f62948153a9586282e653020aed16</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCaseInsensitive</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af4045f11e8887dda083d0122d24df4f6ae7f1d84f9d3ed914e475b63b04d2c3ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCaseSensitive</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af4045f11e8887dda083d0122d24df4f6a911f62948153a9586282e653020aed16</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCaseInsensitive</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af4045f11e8887dda083d0122d24df4f6ae7f1d84f9d3ed914e475b63b04d2c3ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ConstString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a927687036677870519dd1a500e160edb</anchor>
      <arglist>(const char8 *str, int32 length=-1)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ConstString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af313334a239c94dd7ed44bb7a78d4e02</anchor>
      <arglist>(const char16 *str, int32 length=-1)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ConstString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a79f3283eec000755f3ec870a252ea177</anchor>
      <arglist>(const ConstString &amp;str, int32 offset=0, int32 length=-1)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ConstString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a45c99750b35e507bfb2d69a8ee0db3e9</anchor>
      <arglist>(const FVariant &amp;var)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ConstString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ae772e034332b4e5123c75abd3b926e66</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ConstString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ad522b88dd7b89b8b980b084c36293f82</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int32</type>
      <name>length</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aed9da38615ace94c0e53dd4ad946e767</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isEmpty</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>acf82f9b2937375c7b1cf3dccb3df3312</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator const char8 *</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ada4248834fd60a76fc6627bccf5dc960</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator const char16 *</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a21ca283f895ea4013ed0f8af2ab626ec</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>operator[]</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a356c76a37e7adcffc6595745aa4ece47</anchor>
      <arglist>(short idx) const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>operator[]</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af225d4bf871e806df526196612b2f2d0</anchor>
      <arglist>(long idx) const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>operator[]</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a3a8adb9df6c449f7cbb5e67cce322e3b</anchor>
      <arglist>(int idx) const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>operator[]</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a09a37075bcb12c72abda9fb887ee2d47</anchor>
      <arglist>(unsigned short idx) const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>operator[]</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a903864b138c4ea61c7cffe788a6d0087</anchor>
      <arglist>(unsigned long idx) const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>operator[]</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a0e3593cb1c3537ca53339e22c92f02ab</anchor>
      <arglist>(unsigned int idx) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const char8 *</type>
      <name>text8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a8f521eb4befb517ccbb805524cf0f6d9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const char16 *</type>
      <name>text16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ae81468efdbf10b342cc4ef55f952f834</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const char *</type>
      <name>text</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a8bd0e935e0bc021ced707a39a6e82b44</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const void *</type>
      <name>ptr</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a1190cbd432234effc5cdc52349e203ac</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual char8</type>
      <name>getChar8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a667af2582601cf05f6360f779bfffe1a</anchor>
      <arglist>(uint32 index) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual char16</type>
      <name>getChar16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aa83ea4feb441ab820837b70d5eda2072</anchor>
      <arglist>(uint32 index) const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>getChar</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a6a45acdf7364f3a13d056cddf5bd1636</anchor>
      <arglist>(uint32 index) const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>getCharAt</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a0fff14d842c46911af85e125af9d3cdc</anchor>
      <arglist>(uint32 index) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>testChar8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a5931a39bd912ce850f92e42abbb14bc4</anchor>
      <arglist>(uint32 index, char8 c) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>testChar16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ad9b59e0597367959da6913920dd79dcf</anchor>
      <arglist>(uint32 index, char16 c) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>testChar</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af83f3beacc81cbacf3aee859fef32539</anchor>
      <arglist>(uint32 index, char8 c) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>testChar</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a5d2bf50457423819081014fab96cd190</anchor>
      <arglist>(uint32 index, char16 c) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>extract</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a9527af8839efdd33ddf47f43432fcab4</anchor>
      <arglist>(String &amp;result, uint32 idx, int32 n=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>copyTo8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a29e9d379c6687a0ca814e8c380db0ca9</anchor>
      <arglist>(char8 *str, uint32 idx=0, int32 n=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>copyTo16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>afc54bba9b1087cff79c9bf15a7969b49</anchor>
      <arglist>(char16 *str, uint32 idx=0, int32 n=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>copyTo</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ad822b6bfe47f1c4a2ff78ea5f5bf1f25</anchor>
      <arglist>(char *str, uint32 idx=0, int32 n=-1) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copyTo</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>addcf1c0d2f59cb45fceec3d09887a9ae</anchor>
      <arglist>(IStringResult *result) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copyTo</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a23c4de70b1684811a4f2b082f5df3792</anchor>
      <arglist>(IString &amp;string) const</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hash</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a6c9f80b4e5cfed948ee961e1f85b563f</anchor>
      <arglist>(uint32 tsize) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>compareAt</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aa6760149dfe083d5beb2f08942587dcb</anchor>
      <arglist>(uint32 index, const ConstString &amp;str, int32 n=-1, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>compare</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a82a885ba6896ddc2c1ba77b497639bb9</anchor>
      <arglist>(const ConstString &amp;str, int32 n, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>compare</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a981e1c7b984d28e85bd54ea31165e1a2</anchor>
      <arglist>(const ConstString &amp;str, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>naturalCompare</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a0596d70caf53da1ad390e9fb28afabff</anchor>
      <arglist>(const ConstString &amp;str, CompareMode mode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>startsWith</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a3810da8b627c937af8f5d893503e6c82</anchor>
      <arglist>(const ConstString &amp;str, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>endsWith</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ab947e7974deb5cdaf55dcb6897847446</anchor>
      <arglist>(const ConstString &amp;str, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>contains</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a287a9680751f4878ab72e5f1ba955bb8</anchor>
      <arglist>(const ConstString &amp;str, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findLast</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aab9413a67e31ef5eab5947e1c96afd1d</anchor>
      <arglist>(const ConstString &amp;str, int32 n=-1, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findLast</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a4ecb0cc7b206806d489c13d90311022d</anchor>
      <arglist>(char8 c, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findLast</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a1aa4d21b859de7af311fc29a999c7040</anchor>
      <arglist>(char16 c, CompareMode m=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>countOccurences</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a9e910d6f244aa16ef19d40a7626c2e25</anchor>
      <arglist>(char8 c, uint32 startIndex, CompareMode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>countOccurences</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ac225597955da8175197cce72ce4141b5</anchor>
      <arglist>(char16 c, uint32 startIndex, CompareMode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>getFirstDifferent</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a124b545ed153a94c002e032db22bc35d</anchor>
      <arglist>(const ConstString &amp;str, CompareMode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isDigit</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>abb174217e84bce846da12b911695f1dd</anchor>
      <arglist>(uint32 index) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>scanFloat</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af7e48364e8bca44d6ad1aa53167557f8</anchor>
      <arglist>(double &amp;value, uint32 offset=0, bool scanToEnd=true) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>scanInt64</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a7c98274e504654515047b90399cdcb13</anchor>
      <arglist>(int64 &amp;value, uint32 offset=0, bool scanToEnd=true) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>scanUInt64</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af668a757151a1323f0fcd37ecea19e30</anchor>
      <arglist>(uint64 &amp;value, uint32 offset=0, bool scanToEnd=true) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>scanInt32</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af8ace5afb5cf801e81510b2c64598d73</anchor>
      <arglist>(int32 &amp;value, uint32 offset=0, bool scanToEnd=true) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>scanUInt32</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a4000a1ff425649aea8f9f450bd063804</anchor>
      <arglist>(uint32 &amp;value, uint32 offset=0, bool scanToEnd=true) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>scanHex</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a23dc8ed77c745a918945d2cc5cbd0eef</anchor>
      <arglist>(uint8 &amp;value, uint32 offset=0, bool scanToEnd=true) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>getTrailingNumberIndex</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ab877fc292201494ab4be2be190fd5cd1</anchor>
      <arglist>(uint32 width=0) const</arglist>
    </member>
    <member kind="function">
      <type>int64</type>
      <name>getTrailingNumber</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>abd3bd0c45c739a00930d9104bc4f039c</anchor>
      <arglist>(int64 fallback=0) const</arglist>
    </member>
    <member kind="function">
      <type>int64</type>
      <name>getNumber</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a9e50d4a383b9b0048b30296842162152</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>toVariant</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ad7cc90c66e4e214cd03d7f6f737e7be2</anchor>
      <arglist>(FVariant &amp;var) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isWideString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a2569fa33cce1a8256ff4cea575b1829e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isAsciiString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a3329e812c7809a57e3be4946fc63b18e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isNormalized</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aabde9540d808cb7f31588d772c05cb30</anchor>
      <arglist>(UnicodeNormalization=kUnicodeNormC)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findFirst</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a4e230c10f923605ec82493969b6bb273</anchor>
      <arglist>(const ConstString &amp;str, int32 n=-1, CompareMode m=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findFirst</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>adee9de5c767e80bf8d4b0a3d6271370c</anchor>
      <arglist>(char8 c, CompareMode m=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findFirst</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ac9d75370b24a1839a078386a6e746ad5</anchor>
      <arglist>(char16 c, CompareMode m=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findNext</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a011b87c5fa06338481f6db2ed1220189</anchor>
      <arglist>(int32 startIndex, const ConstString &amp;str, int32 n=-1, CompareMode=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findNext</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>afc40cf7970b4652de7f2ef5d572ba132</anchor>
      <arglist>(int32 startIndex, char8 c, CompareMode=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findNext</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ade109f554543eb2f4c5a674d49dd1534</anchor>
      <arglist>(int32 startIndex, char16 c, CompareMode=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findPrev</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aa82963a731a7e3f773d530298180b0c4</anchor>
      <arglist>(int32 startIndex, const ConstString &amp;str, int32 n=-1, CompareMode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findPrev</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aa47dfcfc097efc86b331513b1af28d1b</anchor>
      <arglist>(int32 startIndex, char8 c, CompareMode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findPrev</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a1d268231e3af3641e2c8e118b6055b32</anchor>
      <arglist>(int32 startIndex, char16 c, CompareMode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharSpace</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a558f9643b112a9e296034f4b5c104da4</anchor>
      <arglist>(char8 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharSpace</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>afc1738b9f2705034218d299e33afbb0e</anchor>
      <arglist>(char16 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharAlpha</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a6447bfca9b9d31ed9ce96a35fb987206</anchor>
      <arglist>(char8 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharAlpha</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ac2291ddfae0133dedde40a26dbd51099</anchor>
      <arglist>(char16 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharAlphaNum</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a2b48d0f2b9de6a59ee41f6029f030dfa</anchor>
      <arglist>(char8 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharAlphaNum</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af748287f5024af4c82b3e6225660f88d</anchor>
      <arglist>(char16 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharDigit</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a60d921d7240987538c237a8c5a315e73</anchor>
      <arglist>(char8 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharDigit</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a1561e3f288d0886f47344379e2f0fa49</anchor>
      <arglist>(char16 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharAscii</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af506286b5d395d5b6e5918eba7f71486</anchor>
      <arglist>(char8 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharAscii</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af0143d0862af1c86a44f42708b7d6fce</anchor>
      <arglist>(char16 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharUpper</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a15c5156f789b384961e95ed427c62e9b</anchor>
      <arglist>(char8 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharUpper</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a1735df81440afee9f39ffa303acfcc80</anchor>
      <arglist>(char16 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharLower</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a6288d7b8b62d464b8cda5cdc86054bc8</anchor>
      <arglist>(char8 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isCharLower</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>af69b5c5f85db7bf8e88f94fd541c2eda</anchor>
      <arglist>(char16 character)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanInt64_8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ae46cb29a2fe9015768110c6e8a2bf3a1</anchor>
      <arglist>(const char8 *text, int64 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanInt64_16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a6c893b487dfe24d9678302794e8c08d3</anchor>
      <arglist>(const char16 *text, int64 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanInt64</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ada807776c00d43a2be387ef88614127f</anchor>
      <arglist>(const char *text, int64 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanUInt64_8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ae520c42b0edc50aed97977a73c592f75</anchor>
      <arglist>(const char8 *text, uint64 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanUInt64_16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a2426d33f600572497fb3e4954caaf20d</anchor>
      <arglist>(const char16 *text, uint64 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanUInt64</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a50186e229b1da9121661a54240747ecb</anchor>
      <arglist>(const char *text, uint64 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanInt32_8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a3a230a4b3bd90078a4c7f22080ffad82</anchor>
      <arglist>(const char8 *text, int32 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanInt32_16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a06b4a788c1d6d1f32c3dfaac162b11ac</anchor>
      <arglist>(const char16 *text, int32 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanInt32</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a2911ac52c6c2ad63d4ef0b65e4a0d107</anchor>
      <arglist>(const char *text, int32 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanUInt32_8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a1b111b6a67a73953b9aec2f6afc11661</anchor>
      <arglist>(const char8 *text, uint32 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanUInt32_16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ac19286d8f082ef76774460eb7933b947</anchor>
      <arglist>(const char16 *text, uint32 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanUInt32</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a72534f72b582b95ec762ec61940875f8</anchor>
      <arglist>(const char *text, uint32 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanHex_8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a202229a2718b4c6e8f6720df328b27a0</anchor>
      <arglist>(const char8 *text, uint8 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanHex_16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ad408b57b46dd39329c244d1e59ad3eeb</anchor>
      <arglist>(const char16 *text, uint8 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scanHex</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>adcee4e2680b55bb072b8ea4658dcb775</anchor>
      <arglist>(const char *text, uint8 &amp;value, bool scanToEnd=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char8</type>
      <name>toLower</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aa1bd71cbb5c91ad80af441549e445659</anchor>
      <arglist>(char8 c)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char8</type>
      <name>toUpper</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a1e2802d0abf81576c53da9c30aacf969</anchor>
      <arglist>(char8 c)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char16</type>
      <name>toLower</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a69028c11ba44bc35dff982bb2120f38e</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char16</type>
      <name>toUpper</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a62c158cae29f219f5a14384024dc69a7</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32</type>
      <name>multiByteToWideString</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aae9eb74628356d91bdd0e50426577d02</anchor>
      <arglist>(char16 *dest, const char8 *source, int32 wcharCount, uint32 sourceCodePage=kCP_Default)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32</type>
      <name>wideStringToMultiByte</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a81838f3369304321ab595bc000368f14</anchor>
      <arglist>(char8 *dest, const char16 *source, int32 char8Count, uint32 destCodePage=kCP_Default)</arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>buffer</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a368f7094dc38acca20612bbb392552f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char8 *</type>
      <name>buffer8</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>abb93591ce1f1696318386689faa7627b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char16 *</type>
      <name>buffer16</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a8d780846d765cbb997ce79cafa832cac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>uint32</type>
      <name>len</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a63941c3a03dc9736f3234bb756d9a305</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>uint32</type>
      <name>isWide</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a75060976753e877247e364d1816245d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findFirst</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a4e230c10f923605ec82493969b6bb273</anchor>
      <arglist>(const ConstString &amp;str, int32 n=-1, CompareMode m=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findFirst</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>adee9de5c767e80bf8d4b0a3d6271370c</anchor>
      <arglist>(char8 c, CompareMode m=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findFirst</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ac9d75370b24a1839a078386a6e746ad5</anchor>
      <arglist>(char16 c, CompareMode m=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findNext</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a011b87c5fa06338481f6db2ed1220189</anchor>
      <arglist>(int32 startIndex, const ConstString &amp;str, int32 n=-1, CompareMode=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findNext</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>afc40cf7970b4652de7f2ef5d572ba132</anchor>
      <arglist>(int32 startIndex, char8 c, CompareMode=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findNext</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>ade109f554543eb2f4c5a674d49dd1534</anchor>
      <arglist>(int32 startIndex, char16 c, CompareMode=kCaseSensitive, int32 endIndex=-1) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findPrev</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aa82963a731a7e3f773d530298180b0c4</anchor>
      <arglist>(int32 startIndex, const ConstString &amp;str, int32 n=-1, CompareMode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findPrev</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>aa47dfcfc097efc86b331513b1af28d1b</anchor>
      <arglist>(int32 startIndex, char8 c, CompareMode=kCaseSensitive) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>findPrev</name>
      <anchorfile>classSteinberg_1_1ConstString.html</anchorfile>
      <anchor>a1d268231e3af3641e2c8e118b6055b32</anchor>
      <arglist>(int32 startIndex, char16 c, CompareMode=kCaseSensitive) const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::CommandLine::Description</name>
    <filename>classSteinberg_1_1CommandLine_1_1Description.html</filename>
    <member kind="function">
      <type></type>
      <name>Description</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Description.html</anchorfile>
      <anchor>ad22e14e4e415962ac43d552ed282c9fc</anchor>
      <arglist>(const std::string &amp;name, const std::string &amp;help, const std::string &amp;valueType)</arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>mHelp</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Description.html</anchorfile>
      <anchor>aec239d2835b6febb1c0c7096896f4ee7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>mType</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Description.html</anchorfile>
      <anchor>a3437227e441ef7e6d2b63197b26023ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const std::string</type>
      <name>kBool</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Description.html</anchorfile>
      <anchor>a2d50e9061f301e53c3dd5d51ed45b92b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const std::string</type>
      <name>kString</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Description.html</anchorfile>
      <anchor>a8cd538caaf9f8e5e8a721c6efd58e75f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::CommandLine::Descriptions</name>
    <filename>classSteinberg_1_1CommandLine_1_1Descriptions.html</filename>
    <member kind="function">
      <type>Descriptions &amp;</type>
      <name>addOptions</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Descriptions.html</anchorfile>
      <anchor>ab27a3d9559b5d6fddcbeb9d1474fd349</anchor>
      <arglist>(const std::string &amp;caption=&quot;&quot;, std::initializer_list&lt; Description &gt; &amp;&amp;options={})</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>parse</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Descriptions.html</anchorfile>
      <anchor>af98b844ecc3e57dea64a33273eb40ecb</anchor>
      <arglist>(int ac, char *av[], VariablesMap &amp;result, FilesVector *files=nullptr) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Descriptions.html</anchorfile>
      <anchor>ae2910d69620d530f974b6ee1f94602b8</anchor>
      <arglist>(std::ostream &amp;os) const</arglist>
    </member>
    <member kind="function">
      <type>Descriptions &amp;</type>
      <name>operator()</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Descriptions.html</anchorfile>
      <anchor>ac88217d791c334de62e8f01564ccae24</anchor>
      <arglist>(const std::string &amp;name, const std::string &amp;help)</arglist>
    </member>
    <member kind="function">
      <type>Descriptions &amp;</type>
      <name>operator()</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Descriptions.html</anchorfile>
      <anchor>a4fa6431ffb9b0b95dbe5b25d78f5ea1a</anchor>
      <arglist>(const std::string &amp;name, const Type &amp;inType, std::string help)</arglist>
    </member>
    <member kind="function">
      <type>Descriptions &amp;</type>
      <name>operator()</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1Descriptions.html</anchorfile>
      <anchor>a01418dd3c558865138a493e94428722a</anchor>
      <arglist>(const std::string &amp;name, const std::string &amp;inType, std::string help)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::DisableDispatchingTimers</name>
    <filename>classSteinberg_1_1DisableDispatchingTimers.html</filename>
    <member kind="function">
      <type></type>
      <name>DisableDispatchingTimers</name>
      <anchorfile>classSteinberg_1_1DisableDispatchingTimers.html</anchorfile>
      <anchor>a59be662c23adc93f540b980f63e5c446</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~DisableDispatchingTimers</name>
      <anchorfile>classSteinberg_1_1DisableDispatchingTimers.html</anchorfile>
      <anchor>ac20207d5c81a874be61fcb9621104a0d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Steinberg::FArrayDeleter</name>
    <filename>structSteinberg_1_1FArrayDeleter.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>FArrayDeleter</name>
      <anchorfile>structSteinberg_1_1FArrayDeleter.html</anchorfile>
      <anchor>a43cb3155da3a95365940b11cc42cbba8</anchor>
      <arglist>(T *_arrayToDelete)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FArrayDeleter</name>
      <anchorfile>structSteinberg_1_1FArrayDeleter.html</anchorfile>
      <anchor>a34f230dc8f23cd2085bcc7ccf23f7b3f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>T *</type>
      <name>arrayToDelete</name>
      <anchorfile>structSteinberg_1_1FArrayDeleter.html</anchorfile>
      <anchor>a0af642d63ab48f6c3075a40cba9935e5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Steinberg::FBooleanSetter</name>
    <filename>structSteinberg_1_1FBooleanSetter.html</filename>
    <templarg></templarg>
    <member kind="function">
      <type></type>
      <name>FBooleanSetter</name>
      <anchorfile>structSteinberg_1_1FBooleanSetter.html</anchorfile>
      <anchor>a184b4f0acbd0515534b0a8c161f9ee38</anchor>
      <arglist>(T &amp;_toSet)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FBooleanSetter</name>
      <anchorfile>structSteinberg_1_1FBooleanSetter.html</anchorfile>
      <anchor>ab457975e1c24aeed9865251280280ca2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>T &amp;</type>
      <name>toSet</name>
      <anchorfile>structSteinberg_1_1FBooleanSetter.html</anchorfile>
      <anchor>ad3d8bf7a93c1977f4c20dd6f66bd1f7c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Steinberg::FCloser</name>
    <filename>structSteinberg_1_1FCloser.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>FCloser</name>
      <anchorfile>structSteinberg_1_1FCloser.html</anchorfile>
      <anchor>a46a490c0e086deff18fe1a1d484b2910</anchor>
      <arglist>(T *_obj)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FCloser</name>
      <anchorfile>structSteinberg_1_1FCloser.html</anchorfile>
      <anchor>a2187198e46898e7a63b0db52f9e2bf03</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>T *</type>
      <name>obj</name>
      <anchorfile>structSteinberg_1_1FCloser.html</anchorfile>
      <anchor>a370d397cf44fe716a1016d49f41defe1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Steinberg::FConditionalBoolSetter</name>
    <filename>structSteinberg_1_1FConditionalBoolSetter.html</filename>
    <member kind="function">
      <type></type>
      <name>FConditionalBoolSetter</name>
      <anchorfile>structSteinberg_1_1FConditionalBoolSetter.html</anchorfile>
      <anchor>a46c5ce3d2063724dc2cac0a21fef7529</anchor>
      <arglist>(bool &amp;_toSet, bool condition)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FConditionalBoolSetter</name>
      <anchorfile>structSteinberg_1_1FConditionalBoolSetter.html</anchorfile>
      <anchor>a619a26a5580c9ea4043a887063b8b6b7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>bool &amp;</type>
      <name>toSet</name>
      <anchorfile>structSteinberg_1_1FConditionalBoolSetter.html</anchorfile>
      <anchor>a9ace53f30ba43c34f8512bf5e92d2097</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Steinberg::FDeleter</name>
    <filename>structSteinberg_1_1FDeleter.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>FDeleter</name>
      <anchorfile>structSteinberg_1_1FDeleter.html</anchorfile>
      <anchor>a8d904032d15002f7232451f551053c29</anchor>
      <arglist>(T *_toDelete)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FDeleter</name>
      <anchorfile>structSteinberg_1_1FDeleter.html</anchorfile>
      <anchor>af7025fb8b71b21b2b945a99a326aae46</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>T *</type>
      <name>toDelete</name>
      <anchorfile>structSteinberg_1_1FDeleter.html</anchorfile>
      <anchor>a96acfe8f38200ad7c85bff6c9032fc95</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::FDynLibrary</name>
    <filename>classSteinberg_1_1FDynLibrary.html</filename>
    <base>Steinberg::FObject</base>
    <member kind="function">
      <type></type>
      <name>FDynLibrary</name>
      <anchorfile>classSteinberg_1_1FDynLibrary.html</anchorfile>
      <anchor>a835423ebbf9ed98ac1ff15ae07696dbf</anchor>
      <arglist>(const char *name=nullptr, bool addExtension=true)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FDynLibrary</name>
      <anchorfile>classSteinberg_1_1FDynLibrary.html</anchorfile>
      <anchor>a316c1ee139e497b2bbb0978aae56814d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>init</name>
      <anchorfile>classSteinberg_1_1FDynLibrary.html</anchorfile>
      <anchor>a8f36f17c29d5a7cc5f29677a6093ac5f</anchor>
      <arglist>(const char *name, bool addExtension=true)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>getProcAddress</name>
      <anchorfile>classSteinberg_1_1FDynLibrary.html</anchorfile>
      <anchor>a8060fef6fd902c5aeda63d7d1898e2ff</anchor>
      <arglist>(const char *name)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isLoaded</name>
      <anchorfile>classSteinberg_1_1FDynLibrary.html</anchorfile>
      <anchor>a7ffa9fcfaa7e93495a34c760b8270731</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>unload</name>
      <anchorfile>classSteinberg_1_1FDynLibrary.html</anchorfile>
      <anchor>a506a572d1bf8ada0f9a767c00e580f53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>getPlatformInstance</name>
      <anchorfile>classSteinberg_1_1FDynLibrary.html</anchorfile>
      <anchor>afb23c99f552263593ee364f9c2c3b0b9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable">
      <type>OBJ_METHODS(FDynLibrary, FObject) protected void *</type>
      <name>instance</name>
      <anchorfile>classSteinberg_1_1FDynLibrary.html</anchorfile>
      <anchor>a65d5dd0c24f120a3b2c0b6d2e2038d58</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::FMallocReleaser</name>
    <filename>classSteinberg_1_1FMallocReleaser.html</filename>
    <member kind="function">
      <type></type>
      <name>FMallocReleaser</name>
      <anchorfile>classSteinberg_1_1FMallocReleaser.html</anchorfile>
      <anchor>a005dcedf7047776009c4437b8219a407</anchor>
      <arglist>(void *_data)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FMallocReleaser</name>
      <anchorfile>classSteinberg_1_1FMallocReleaser.html</anchorfile>
      <anchor>aa6a5aeac7205ddffc9a5d5515479f3e7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>void *</type>
      <name>data</name>
      <anchorfile>classSteinberg_1_1FMallocReleaser.html</anchorfile>
      <anchor>a735984d41155bc1032e09bece8f8d66d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Steinberg::FNuller</name>
    <filename>structSteinberg_1_1FNuller.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>FNuller</name>
      <anchorfile>structSteinberg_1_1FNuller.html</anchorfile>
      <anchor>a9fd1a507ebc6f2535cd3130e5bf7cb04</anchor>
      <arglist>(T &amp;_toNull)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FNuller</name>
      <anchorfile>structSteinberg_1_1FNuller.html</anchorfile>
      <anchor>ab9191536eaf9f59f8fe55bad97cf001d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>T &amp;</type>
      <name>toNull</name>
      <anchorfile>structSteinberg_1_1FNuller.html</anchorfile>
      <anchor>ab041bd5c32608007e7a8f433b9d9e861</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::FObject</name>
    <filename>classSteinberg_1_1FObject.html</filename>
    <member kind="function">
      <type></type>
      <name>FObject</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a437422592a0d826d2c3962fb7fff8404</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>FObject</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a0df169f35093cd89cf30109783cc900b</anchor>
      <arglist>(const FObject &amp;)</arglist>
    </member>
    <member kind="function">
      <type>FObject &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>af6c944b30efba40e7bbd2ab51df630b5</anchor>
      <arglist>(const FObject &amp;)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~FObject</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a6bd63fda63882c46b7c08409f68ad23b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual FClassID</type>
      <name>isA</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>aae90f8c54876fd8f1a191a44144d66c7</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>isA</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a472e34bafb6415ae1054b6e4ffd9e3f5</anchor>
      <arglist>(FClassID s) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>isTypeOf</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a7828bf1972d5be433fdd6320e109f3be</anchor>
      <arglist>(FClassID s, bool=true) const</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>getRefCount</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a127b5517d355d7870c5bc96d52ef9ef0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>FUnknown *</type>
      <name>unknownCast</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a802669246b5bbc85558f472a9c179af0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tresult</type>
      <name>queryInterface</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a9a18a9f05017946de2dc6942fc7b55b0</anchor>
      <arglist>(const TUID _iid, void **obj) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>addRef</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>ab71f235d96a97eb6e56272b090694878</anchor>
      <arglist>() SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>release</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a936c8aef3cc4614fc83ca91233e99521</anchor>
      <arglist>() SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>update</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>aeeeb082a829ee7b136a730a2a4029103</anchor>
      <arglist>(FUnknown *, int32) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>addDependent</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>aad64a1f592d31c920bc72505ab860576</anchor>
      <arglist>(IDependent *dep)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>removeDependent</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a4a343976011506b85607300a4bd97832</anchor>
      <arglist>(IDependent *dep)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>changed</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a72f44c7bf8ec3cab12b95dde13542194</anchor>
      <arglist>(int32 msg=kChanged)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>deferUpdate</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>ad0cb02215c1c6e44b53f177e7844a9f5</anchor>
      <arglist>(int32 msg=kChanged)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>updateDone</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a0be193466912191a8ebbd75354edd935</anchor>
      <arglist>(int32)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>isEqualInstance</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a5a7c5326199ffb542719387b078ed6da</anchor>
      <arglist>(FUnknown *d)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static FClassID</type>
      <name>getFClassID</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a61fe4989547b521dfcac968665c376d9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setUpdateHandler</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>abf312648bf542a9d35fd34e1564b07cb</anchor>
      <arglist>(IUpdateHandler *handler)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static IUpdateHandler *</type>
      <name>getUpdateHandler</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>ae2039f0b388fde926f5adeba0111e3d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>classIDsEqual</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>af59fc037df86c95566a2f9874be552a4</anchor>
      <arglist>(FClassID ci1, FClassID ci2)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static FObject *</type>
      <name>unknownToObject</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>a0a25d706b3e3dabc9e4c0d026f925d73</anchor>
      <arglist>(FUnknown *unknown)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const FUID</type>
      <name>iid</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>ae78a44399218c930be44d3aa46521d0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int32</type>
      <name>refCount</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>abc6aaa0595fa788544c4b18e9bf93850</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static IUpdateHandler *</type>
      <name>gUpdateHandler</name>
      <anchorfile>classSteinberg_1_1FObject.html</anchorfile>
      <anchor>acee1b675f92ac47a457fc7692dfd3a67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Steinberg::FPtrNuller</name>
    <filename>structSteinberg_1_1FPtrNuller.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>FPtrNuller</name>
      <anchorfile>structSteinberg_1_1FPtrNuller.html</anchorfile>
      <anchor>a965668bcd07a18ad817d591cdfe2c3d5</anchor>
      <arglist>(T *&amp;_toNull)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FPtrNuller</name>
      <anchorfile>structSteinberg_1_1FPtrNuller.html</anchorfile>
      <anchor>a31801922b2fb31bd9d56ef48f1abe8da</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>T *&amp;</type>
      <name>toNull</name>
      <anchorfile>structSteinberg_1_1FPtrNuller.html</anchorfile>
      <anchor>a791d6bee42c812d34ecfcac333ab33c0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::FStreamer</name>
    <filename>classSteinberg_1_1FStreamer.html</filename>
    <member kind="function">
      <type></type>
      <name>FStreamer</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a326b593fe85c31981865746c8cf0303d</anchor>
      <arglist>(int16 byteOrder=BYTEORDER)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~FStreamer</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a939394fecf611c7d3b31016175d4e0e6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>skip</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a28410ce59c7f96b01d34cbb996ba9d07</anchor>
      <arglist>(uint32 bytes)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>pad</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a63372447ec8258db1b7d5e2b624c6ec0</anchor>
      <arglist>(uint32 bytes)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual TSize</type>
      <name>readRaw</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>af7b1f1806480071cbe0822ca3b19240e</anchor>
      <arglist>(void *, TSize)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual TSize</type>
      <name>writeRaw</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad9301dcce91b6366a1ebf1673998c673</anchor>
      <arglist>(const void *, TSize)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual int64</type>
      <name>seek</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a80ed73a703e084b355627d8966daf77f</anchor>
      <arglist>(int64, FSeekMode)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual int64</type>
      <name>tell</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a1ba197f3c51297c9c18e64a27fb51c19</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setByteOrder</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>afd8f7d6bb97d1020c32fcb1beb49986a</anchor>
      <arglist>(int32 e)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>getByteOrder</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ac5b0871ff8d06951e2df5194e93fdbbf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeChar8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a998c6902fdd8b21bbebc626dd4d63120</anchor>
      <arglist>(char8)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readChar8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a49117f69772f84b75ac6eb3ca72c4f33</anchor>
      <arglist>(char8 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeUChar8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a3d2ae068d6eece63ec3f95bc42efb5ed</anchor>
      <arglist>(unsigned char)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readUChar8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a050044d61e58a580dbe8955e78e59715</anchor>
      <arglist>(unsigned char &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeChar16</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a3843e29715ce30dbbc2cfd15e0e213d3</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readChar16</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a8367a9a0969af33c39aa4e307bf80708</anchor>
      <arglist>(char16 &amp;c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad77412c80fcf33ff6b234644f6049953</anchor>
      <arglist>(int8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>aaffe413db58e603c4e185a89bbaa5b1e</anchor>
      <arglist>(int8 &amp;c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt8u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>aeb4e434ddc84c1ed48329ca603d97c58</anchor>
      <arglist>(uint8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt8u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a0b787b493ecdccc6978a34b42945ef5f</anchor>
      <arglist>(uint8 &amp;c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt16</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a5370cf9ca403ed7933976cf59423d309</anchor>
      <arglist>(int16)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt16</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a3254224fd4c495000e4b18538f42ece1</anchor>
      <arglist>(int16 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt16Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a51f8744e0f55375577c51c062168f79f</anchor>
      <arglist>(const int16 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt16Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a27b993ce8d89e96688949124845dad2f</anchor>
      <arglist>(int16 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt16u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad6ad0e47c8f4c10459184131d10bd1ef</anchor>
      <arglist>(uint16)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt16u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a7d425607375843e893b80c65a67c165d</anchor>
      <arglist>(uint16 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt16uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a242bb719494fb5ce34101f781630f218</anchor>
      <arglist>(const uint16 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt16uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad6ad574460786ee6a7c4f7fbc2261690</anchor>
      <arglist>(uint16 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt32</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a0ade9fcb607318bf3e2e3e61128dc5dd</anchor>
      <arglist>(int32)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt32</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>afdc3408bcec5a81b9ce74521836bc97f</anchor>
      <arglist>(int32 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt32Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a3b9f76b17804e930c08aa0801329498c</anchor>
      <arglist>(const int32 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt32Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a7a832e7546b61a35e0d80c7cfcddc896</anchor>
      <arglist>(int32 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt32u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>acd7e32e72765009e13ad7dbbcfd7e1ee</anchor>
      <arglist>(uint32)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt32u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>abe21610fed6d57bde1d0c99896ca9545</anchor>
      <arglist>(uint32 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt32uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a8afcc5dbaa740a01e9efdb87728c0f9d</anchor>
      <arglist>(const uint32 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt32uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ac903b1420b673af2b7c6f4502c4a8157</anchor>
      <arglist>(uint32 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt64</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a8990f73a83e3d804d15b65617330a0a8</anchor>
      <arglist>(int64)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt64</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a6529510e2a381d0e01f508d7a4157a71</anchor>
      <arglist>(int64 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt64Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a0ac8492c1507e7270e81ec46244f38dd</anchor>
      <arglist>(const int64 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt64Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a5b64179d43d284a94d222f4a9b64fd6b</anchor>
      <arglist>(int64 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt64u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ae36bcbad11f652166fc31678095b232e</anchor>
      <arglist>(uint64)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt64u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a17565258ffde08c20df60711320854ec</anchor>
      <arglist>(uint64 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt64uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a079a7a951eee3154e9e89808d02ecc3d</anchor>
      <arglist>(const uint64 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt64uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>acdb427c2448fc4fb7674a0aa1e80dab3</anchor>
      <arglist>(uint64 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeFloat</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a199a188e7f68fb5b5fe509ecbf4b4606</anchor>
      <arglist>(float)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readFloat</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a96a61e2accf8796303c0f37c2d626b77</anchor>
      <arglist>(float &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeFloatArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a034a8ce9c05cf23de6a80d273fd8f348</anchor>
      <arglist>(const float *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readFloatArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a725fcef271dd11d905db76d21098f6eb</anchor>
      <arglist>(float *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeDouble</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a60ef5ff94413517ac820dcae06812f5b</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readDouble</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad1c67fedd9d297f986ebd22a7d3082c0</anchor>
      <arglist>(double &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeDoubleArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a83b6820c68b00e34f496e31563c9eb88</anchor>
      <arglist>(const double *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readDoubleArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a323cfa378914ea23f11731826b9a79e9</anchor>
      <arglist>(double *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeBool</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a51e2b25f25db4ceab194d6b3ef65288c</anchor>
      <arglist>(bool)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readBool</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>aed6f8d7c8792eff4db6c1403b33bbaa4</anchor>
      <arglist>(bool &amp;)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int16</type>
      <name>byteOrder</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a1f29a6ebae4e2b64b0ac03ce73924b46</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual TSize</type>
      <name>readRaw</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>af7b1f1806480071cbe0822ca3b19240e</anchor>
      <arglist>(void *, TSize)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual TSize</type>
      <name>writeRaw</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad9301dcce91b6366a1ebf1673998c673</anchor>
      <arglist>(const void *, TSize)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual int64</type>
      <name>seek</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a80ed73a703e084b355627d8966daf77f</anchor>
      <arglist>(int64, FSeekMode)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual int64</type>
      <name>tell</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a1ba197f3c51297c9c18e64a27fb51c19</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setByteOrder</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>afd8f7d6bb97d1020c32fcb1beb49986a</anchor>
      <arglist>(int32 e)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>getByteOrder</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ac5b0871ff8d06951e2df5194e93fdbbf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeChar8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a998c6902fdd8b21bbebc626dd4d63120</anchor>
      <arglist>(char8)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readChar8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a49117f69772f84b75ac6eb3ca72c4f33</anchor>
      <arglist>(char8 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeUChar8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a3d2ae068d6eece63ec3f95bc42efb5ed</anchor>
      <arglist>(unsigned char)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readUChar8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a050044d61e58a580dbe8955e78e59715</anchor>
      <arglist>(unsigned char &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeChar16</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a3843e29715ce30dbbc2cfd15e0e213d3</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readChar16</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a8367a9a0969af33c39aa4e307bf80708</anchor>
      <arglist>(char16 &amp;c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad77412c80fcf33ff6b234644f6049953</anchor>
      <arglist>(int8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>aaffe413db58e603c4e185a89bbaa5b1e</anchor>
      <arglist>(int8 &amp;c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt8u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>aeb4e434ddc84c1ed48329ca603d97c58</anchor>
      <arglist>(uint8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt8u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a0b787b493ecdccc6978a34b42945ef5f</anchor>
      <arglist>(uint8 &amp;c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt16</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a5370cf9ca403ed7933976cf59423d309</anchor>
      <arglist>(int16)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt16</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a3254224fd4c495000e4b18538f42ece1</anchor>
      <arglist>(int16 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt16Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a51f8744e0f55375577c51c062168f79f</anchor>
      <arglist>(const int16 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt16Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a27b993ce8d89e96688949124845dad2f</anchor>
      <arglist>(int16 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt16u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad6ad0e47c8f4c10459184131d10bd1ef</anchor>
      <arglist>(uint16)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt16u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a7d425607375843e893b80c65a67c165d</anchor>
      <arglist>(uint16 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt16uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a242bb719494fb5ce34101f781630f218</anchor>
      <arglist>(const uint16 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt16uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad6ad574460786ee6a7c4f7fbc2261690</anchor>
      <arglist>(uint16 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt32</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a0ade9fcb607318bf3e2e3e61128dc5dd</anchor>
      <arglist>(int32)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt32</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>afdc3408bcec5a81b9ce74521836bc97f</anchor>
      <arglist>(int32 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt32Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a3b9f76b17804e930c08aa0801329498c</anchor>
      <arglist>(const int32 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt32Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a7a832e7546b61a35e0d80c7cfcddc896</anchor>
      <arglist>(int32 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt32u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>acd7e32e72765009e13ad7dbbcfd7e1ee</anchor>
      <arglist>(uint32)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt32u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>abe21610fed6d57bde1d0c99896ca9545</anchor>
      <arglist>(uint32 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt32uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a8afcc5dbaa740a01e9efdb87728c0f9d</anchor>
      <arglist>(const uint32 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt32uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ac903b1420b673af2b7c6f4502c4a8157</anchor>
      <arglist>(uint32 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt64</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a8990f73a83e3d804d15b65617330a0a8</anchor>
      <arglist>(int64)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt64</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a6529510e2a381d0e01f508d7a4157a71</anchor>
      <arglist>(int64 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt64Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a0ac8492c1507e7270e81ec46244f38dd</anchor>
      <arglist>(const int64 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt64Array</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a5b64179d43d284a94d222f4a9b64fd6b</anchor>
      <arglist>(int64 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt64u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ae36bcbad11f652166fc31678095b232e</anchor>
      <arglist>(uint64)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt64u</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a17565258ffde08c20df60711320854ec</anchor>
      <arglist>(uint64 &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeInt64uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a079a7a951eee3154e9e89808d02ecc3d</anchor>
      <arglist>(const uint64 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readInt64uArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>acdb427c2448fc4fb7674a0aa1e80dab3</anchor>
      <arglist>(uint64 *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeFloat</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a199a188e7f68fb5b5fe509ecbf4b4606</anchor>
      <arglist>(float)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readFloat</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a96a61e2accf8796303c0f37c2d626b77</anchor>
      <arglist>(float &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeFloatArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a034a8ce9c05cf23de6a80d273fd8f348</anchor>
      <arglist>(const float *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readFloatArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a725fcef271dd11d905db76d21098f6eb</anchor>
      <arglist>(float *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeDouble</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a60ef5ff94413517ac820dcae06812f5b</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readDouble</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>ad1c67fedd9d297f986ebd22a7d3082c0</anchor>
      <arglist>(double &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeDoubleArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a83b6820c68b00e34f496e31563c9eb88</anchor>
      <arglist>(const double *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readDoubleArray</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a323cfa378914ea23f11731826b9a79e9</anchor>
      <arglist>(double *array, int32 count)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeBool</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a51e2b25f25db4ceab194d6b3ef65288c</anchor>
      <arglist>(bool)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readBool</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>aed6f8d7c8792eff4db6c1403b33bbaa4</anchor>
      <arglist>(bool &amp;)</arglist>
    </member>
    <member kind="function">
      <type>TSize</type>
      <name>writeString8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a863db218e6a751e68deaed25f39c2b20</anchor>
      <arglist>(const char8 *ptr, bool terminate=false)</arglist>
    </member>
    <member kind="function">
      <type>TSize</type>
      <name>readString8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a38a8c350f3e1bd0af0fd8dfc249fa09b</anchor>
      <arglist>(char8 *ptr, TSize size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeStr8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a4d15efd853e1aaa127fb1b13c7c92485</anchor>
      <arglist>(const char8 *ptr)</arglist>
    </member>
    <member kind="function">
      <type>char8 *</type>
      <name>readStr8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a51bb89aefae6e4e19d87f10d964da75f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>writeStringUtf8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>afb8d3abe460f1380f258137419ee5d10</anchor>
      <arglist>(const char *ptr)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>readStringUtf8</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a6f192015be51ec795ab0b07ee1bd5e17</anchor>
      <arglist>(char *ptr, int32 maxSize)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32</type>
      <name>getStr8Size</name>
      <anchorfile>classSteinberg_1_1FStreamer.html</anchorfile>
      <anchor>a59d0c7f0e826e1e043b6c25dd1cb183d</anchor>
      <arglist>(const char8 *ptr)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::FStreamSizeHolder</name>
    <filename>classSteinberg_1_1FStreamSizeHolder.html</filename>
    <member kind="function">
      <type></type>
      <name>FStreamSizeHolder</name>
      <anchorfile>classSteinberg_1_1FStreamSizeHolder.html</anchorfile>
      <anchor>a2acf6a3a07f1a39929e171f3db5f3e92</anchor>
      <arglist>(FStreamer &amp;s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>beginWrite</name>
      <anchorfile>classSteinberg_1_1FStreamSizeHolder.html</anchorfile>
      <anchor>a27d15dc56498ede0bb029d7a17c1f4e9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>endWrite</name>
      <anchorfile>classSteinberg_1_1FStreamSizeHolder.html</anchorfile>
      <anchor>a82e84d6d19330d005490af6a0a18951e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>beginRead</name>
      <anchorfile>classSteinberg_1_1FStreamSizeHolder.html</anchorfile>
      <anchor>a404df28133b15e6aeff1272dc319a75a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>endRead</name>
      <anchorfile>classSteinberg_1_1FStreamSizeHolder.html</anchorfile>
      <anchor>a1143ed396d1273198dd270fd13a1d1fb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>FStreamer &amp;</type>
      <name>stream</name>
      <anchorfile>classSteinberg_1_1FStreamSizeHolder.html</anchorfile>
      <anchor>a826abaebde6da276f12b3250c52ad90a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int64</type>
      <name>sizePos</name>
      <anchorfile>classSteinberg_1_1FStreamSizeHolder.html</anchorfile>
      <anchor>a4ed2befe00e711ba9345b04f59d18a69</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::IBStreamer</name>
    <filename>classSteinberg_1_1IBStreamer.html</filename>
    <base>Steinberg::FStreamer</base>
    <member kind="function">
      <type></type>
      <name>IBStreamer</name>
      <anchorfile>classSteinberg_1_1IBStreamer.html</anchorfile>
      <anchor>a845d37e3a5e1ef478b7c44527a8ebd9c</anchor>
      <arglist>(IBStream *stream, int16 byteOrder=BYTEORDER)</arglist>
    </member>
    <member kind="function">
      <type>IBStream *</type>
      <name>getStream</name>
      <anchorfile>classSteinberg_1_1IBStreamer.html</anchorfile>
      <anchor>aa119920e997774dcbfdcabeffcbc231b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TSize</type>
      <name>readRaw</name>
      <anchorfile>classSteinberg_1_1IBStreamer.html</anchorfile>
      <anchor>aa597b90907c098d23d7a15daf951528b</anchor>
      <arglist>(void *, TSize) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>TSize</type>
      <name>writeRaw</name>
      <anchorfile>classSteinberg_1_1IBStreamer.html</anchorfile>
      <anchor>a27a86b2c85316f0677a4bd40296b1494</anchor>
      <arglist>(const void *, TSize) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>int64</type>
      <name>seek</name>
      <anchorfile>classSteinberg_1_1IBStreamer.html</anchorfile>
      <anchor>a1120bb884a96a4c670b8d2650f8659c1</anchor>
      <arglist>(int64, FSeekMode) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>int64</type>
      <name>tell</name>
      <anchorfile>classSteinberg_1_1IBStreamer.html</anchorfile>
      <anchor>aea7097a277d6bf8ccf33c9f07ddad79a</anchor>
      <arglist>() SMTG_OVERRIDE</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>IBStream *</type>
      <name>stream</name>
      <anchorfile>classSteinberg_1_1IBStreamer.html</anchorfile>
      <anchor>a78d49eed9e150f042fe1e0434fb52102</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::ITimerCallback</name>
    <filename>classSteinberg_1_1ITimerCallback.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ITimerCallback</name>
      <anchorfile>classSteinberg_1_1ITimerCallback.html</anchorfile>
      <anchor>af00a1cccbd3849f94399eb9ddb8367b1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>onTimer</name>
      <anchorfile>classSteinberg_1_1ITimerCallback.html</anchorfile>
      <anchor>a99358b05e5917c71cb033737c57bed49</anchor>
      <arglist>(Timer *timer)=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::IUpdateManager</name>
    <filename>classSteinberg_1_1IUpdateManager.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual tresult</type>
      <name>cancelUpdates</name>
      <anchorfile>classSteinberg_1_1IUpdateManager.html</anchorfile>
      <anchor>a1a25afc436e4a6a71ea723fb87dd5e17</anchor>
      <arglist>(FUnknown *object)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual tresult</type>
      <name>triggerDeferedUpdates</name>
      <anchorfile>classSteinberg_1_1IUpdateManager.html</anchorfile>
      <anchor>a3de8251112637dff71d770742e2d9990</anchor>
      <arglist>(FUnknown *object=nullptr)=0</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const FUID</type>
      <name>iid</name>
      <anchorfile>classSteinberg_1_1IUpdateManager.html</anchorfile>
      <anchor>ae78a44399218c930be44d3aa46521d0e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::String</name>
    <filename>classSteinberg_1_1String.html</filename>
    <base>Steinberg::ConstString</base>
    <member kind="enumeration">
      <type></type>
      <name>CharGroup</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>af76aa92cca3a7f4f1cc9bfd804a3c873</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSpace</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>af76aa92cca3a7f4f1cc9bfd804a3c873aab823bcc5d8d26fafa6ada800ab21f74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kNotAlphaNum</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>af76aa92cca3a7f4f1cc9bfd804a3c873a7e67f6171578b087aaa7b475ccf1b7a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kNotAlpha</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>af76aa92cca3a7f4f1cc9bfd804a3c873a34415afe7e8fb63d801a1fce28628e8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSpace</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>af76aa92cca3a7f4f1cc9bfd804a3c873aab823bcc5d8d26fafa6ada800ab21f74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kNotAlphaNum</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>af76aa92cca3a7f4f1cc9bfd804a3c873a7e67f6171578b087aaa7b475ccf1b7a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kNotAlpha</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>af76aa92cca3a7f4f1cc9bfd804a3c873a34415afe7e8fb63d801a1fce28628e8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a8db865e64c3c3f65ec6a0b6fd8c09a94</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>aa9bbe6b09b629570004c874e08f76615</anchor>
      <arglist>(const char8 *str, MBCodePage codepage, int32 n=-1, bool isTerminated=true)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a9e3843f45b00dbd34481e62c1ecaf782</anchor>
      <arglist>(const char8 *str, int32 n=-1, bool isTerminated=true)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ada7aea976019ca074da1c9f91a8c05b0</anchor>
      <arglist>(const char16 *str, int32 n=-1, bool isTerminated=true)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a1733e560575975097871b5aed67a2c01</anchor>
      <arglist>(const String &amp;str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a49f71cca276a33db442c6d583d2f9b8f</anchor>
      <arglist>(const ConstString &amp;str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>af9b8fc3f312ec00a3f2ed3cade53d6db</anchor>
      <arglist>(const FVariant &amp;var)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a59f9b66e14775bdb1662cd88074f80d5</anchor>
      <arglist>(IString *str)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~String</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a998ab66c7adb23a76179eec57d05de9d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>updateLength</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>aeaaaf7e3d412ec741fc91722eaaceb13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char8 *</type>
      <name>text8</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a3b3a7237b92adc53af697ef456f977f2</anchor>
      <arglist>() const SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>const char16 *</type>
      <name>text16</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ac96d94e5b3d94215b44300b306f2b0f8</anchor>
      <arglist>() const SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>char8</type>
      <name>getChar8</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a291639b68e8f10c12caa8b1209e46214</anchor>
      <arglist>(uint32 index) const SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>char16</type>
      <name>getChar16</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>aff4625759b55de2a0bef9be71021c32b</anchor>
      <arglist>(uint32 index) const SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setChar8</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a9821aa4f05d40a996f008636dbb0326c</anchor>
      <arglist>(uint32 index, char8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setChar16</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a579d2fd2b9b46c78bdd5383da12ee020</anchor>
      <arglist>(uint32 index, char16 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setChar</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ac141e6f97efe87a6952acae14cd938a1</anchor>
      <arglist>(uint32 index, char8 c)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setChar</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a23bc2b1e6f1dec3921353722c4fbd2e6</anchor>
      <arglist>(uint32 index, char16 c)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ae2055bf8225e1ae1062abf2b2f63503c</anchor>
      <arglist>(const char8 *str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a9762fc85a717c44f1772dfd1933894b1</anchor>
      <arglist>(const char16 *str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a2d987a6d658b7e43497c9521ee809ea3</anchor>
      <arglist>(const ConstString &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>aa7694556e438408131523c949aeecdd0</anchor>
      <arglist>(const String &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a47163cdae5fe81ac503214ea71360686</anchor>
      <arglist>(char8 c)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a0b5b8173bdf45815f7ed64ae5a4235a2</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>assign</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a72ebe09b8abd759e9acb67d8fb44a104</anchor>
      <arglist>(const ConstString &amp;str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>assign</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ad182eaf955c0d87fda7047bfe8c8586e</anchor>
      <arglist>(const char8 *str, int32 n=-1, bool isTerminated=true)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>assign</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a155c3b3d218859544ed3ec4691b5f694</anchor>
      <arglist>(const char16 *str, int32 n=-1, bool isTerminated=true)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>assign</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a305b80fea895a175ff15dfa9fca9dbdc</anchor>
      <arglist>(char8 c, int32 n=1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>assign</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ae4e046ac0b58d1bac992d9316d8ff088</anchor>
      <arglist>(char16 c, int32 n=1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>append</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a7f5057b04cc5fd42d4e841560c48ce17</anchor>
      <arglist>(const ConstString &amp;str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>append</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a8ce343c3a11cb06f11ceb3e543e7bc03</anchor>
      <arglist>(const char8 *str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>append</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a96cbbb5efbaffcbb1f48f12dda2b8581</anchor>
      <arglist>(const char16 *str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>append</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a03eb941bf059e950699670e38bb54a94</anchor>
      <arglist>(const char8 c, int32 n=1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>append</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a5c4d9baf29b4ea315f447ad52d6c5239</anchor>
      <arglist>(const char16 c, int32 n=1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>insertAt</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ab4203737065fd753214e0b65d61a874d</anchor>
      <arglist>(uint32 idx, const ConstString &amp;str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>insertAt</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>aea25de79cbc2237412bd253cdc2836be</anchor>
      <arglist>(uint32 idx, const char8 *str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>insertAt</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a5afc7b36bf978427835538018f6f6bc5</anchor>
      <arglist>(uint32 idx, const char16 *str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>insertAt</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a48cffef1dd37fc734c322b8366bf45fe</anchor>
      <arglist>(uint32 idx, char8 c)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>insertAt</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a92cfef5cae8cad95b4420998cc1298c3</anchor>
      <arglist>(uint32 idx, char16 c)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a8c09167d2a0ffed51ebea0865b3e16e9</anchor>
      <arglist>(const String &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a0bc1eea72b69a361d5bee4c854018d07</anchor>
      <arglist>(const ConstString &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>acd7565e31ea9c44072a2de110f214c88</anchor>
      <arglist>(const char8 *str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ab59e7ce2cba8e051f7f529136b697948</anchor>
      <arglist>(const char16 *str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a7f29feb0fa3ced3574c7bbf3cded1ab4</anchor>
      <arglist>(const char8 c)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator+=</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ac40b69e1365cf11d0fdde38466acb6df</anchor>
      <arglist>(const char16 c)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>replace</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ac1d26f09a0f4f6c084f03b843bc11c19</anchor>
      <arglist>(uint32 idx, int32 n1, const ConstString &amp;str, int32 n2=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>replace</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a2d2030439e7cb36eb5674ef6c9525565</anchor>
      <arglist>(uint32 idx, int32 n1, const char8 *str, int32 n2=-1)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>replace</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a54eb203ea602d6ffea70b28070675060</anchor>
      <arglist>(uint32 idx, int32 n1, const char16 *str, int32 n2=-1)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>replace</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ab6fe569adecb2d469eacfc9d60cd0dd8</anchor>
      <arglist>(const char8 *toReplace, const char8 *toReplaceWith, bool all=false, CompareMode m=kCaseSensitive)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>replace</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a899153b5e15cf1a5fb89ea20284c517e</anchor>
      <arglist>(const char16 *toReplace, const char16 *toReplaceWith, bool all=false, CompareMode m=kCaseSensitive)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceChars8</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a43e88f4cca43c27ed5d24ef94e5af91b</anchor>
      <arglist>(const char8 *toReplace, char8 toReplaceBy)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceChars16</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a58a8e5496343516f41a73c2230e86122</anchor>
      <arglist>(const char16 *toReplace, char16 toReplaceBy)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceChars8</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a61ff75394ec5a0912c1308c23cd07d9e</anchor>
      <arglist>(char8 toReplace, char8 toReplaceBy)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceChars16</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a512d9ef3766c15768ae7f75debeb4d11</anchor>
      <arglist>(char16 toReplace, char16 toReplaceBy)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a356590acdce80db07f9eb77b6de98324</anchor>
      <arglist>(char8 toReplace, char8 toReplaceBy)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a8a25396b718a5609eb42fc1d61ea1fa4</anchor>
      <arglist>(char16 toReplace, char16 toReplaceBy)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a6d0b5c39ee8a41d84da38f83f4a6606e</anchor>
      <arglist>(const char8 *toReplace, char8 toReplaceBy)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>replaceChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a674fdd0d0499a7bc150e9b3d379164f0</anchor>
      <arglist>(const char16 *toReplace, char16 toReplaceBy)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>remove</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a01c841b84afa55d724019512fd7af8f5</anchor>
      <arglist>(uint32 index=0, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>trim</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a80d652fd1a09fe0c4e667c670f9b2dd5</anchor>
      <arglist>(CharGroup mode=kSpace)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a5c04ba944704a95ddd92fc9d7afd10e1</anchor>
      <arglist>(CharGroup mode=kSpace)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeChars8</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a1936a7b4d4ccd408551b8a0fdf2a7c9f</anchor>
      <arglist>(const char8 *which)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeChars16</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>adcd7ed15ae18cf7c3178ac36e6f4e36e</anchor>
      <arglist>(const char16 *which)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeChars8</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a79629fec102b32f037dfb109552d4a3d</anchor>
      <arglist>(const char8 which)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeChars16</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>aa82260d28382fd80696eb4e42e4bd21c</anchor>
      <arglist>(const char16 which)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a0056c28bddfa9d61b7b18c498fc73157</anchor>
      <arglist>(const char8 *which)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a029e8327149ae92c67968bc0a58ad9fc</anchor>
      <arglist>(const char16 *which)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a2e8359042c52c36d914af3592145519c</anchor>
      <arglist>(const char8 which)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeChars</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a507ff5666ca801779d21fca6569bd60f</anchor>
      <arglist>(const char16 which)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>removeSubString</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a51c672fff69360fbef073080071fb7b0</anchor>
      <arglist>(const ConstString &amp;subString, bool allOccurences=true)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>printf</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ac75a7887729b9c2e5684363dc687f335</anchor>
      <arglist>(const char8 *format,...)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>printf</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a537197b902c970c72b087ff9f869be2c</anchor>
      <arglist>(const char16 *format,...)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>vprintf</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a09b2e8a8b96a7cdf1c149514d773d7dc</anchor>
      <arglist>(const char8 *format, va_list args)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>vprintf</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ad45799a3ed3869d8996555f78175c4de</anchor>
      <arglist>(const char16 *format, va_list args)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>printInt64</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a76fce2f426ae58e98a6c9722a7a219a2</anchor>
      <arglist>(int64 value)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>printFloat</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a02ade24a8128f57c7c1682f7c4e193e2</anchor>
      <arglist>(double value)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>incrementTrailingNumber</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a784e9e7df7086386504a8935b9e39879</anchor>
      <arglist>(uint32 width=2, char separator=STR(&apos; &apos;), uint32 minNumber=1, bool applyOnlyFormat=false)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>fromVariant</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a3b4c47d6b39b4499f0fe5bf9c2e289a7</anchor>
      <arglist>(const FVariant &amp;var)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>toVariant</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ad7cc90c66e4e214cd03d7f6f737e7be2</anchor>
      <arglist>(FVariant &amp;var) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>fromAttributes</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>afa3a283b87647a8898b8847993972884</anchor>
      <arglist>(IAttributes *a, IAttrID attrID)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>toAttributes</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>afdfaa3b6d6a266c6dc41ad1843a5f804</anchor>
      <arglist>(IAttributes *a, IAttrID attrID)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>swapContent</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a57f507c68a120a81db668613a1ee1b73</anchor>
      <arglist>(String &amp;s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>take</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ae23d51189d31f93126db6c62eddabe86</anchor>
      <arglist>(String &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>take</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>aae43edfabf05c02e0f0fc7c64056f382</anchor>
      <arglist>(void *_buffer, bool wide)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>pass</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>adec058d1c3e667b7f0f964b11265033c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>passToVariant</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ae2263181a009962d02fd25b33b05a1d9</anchor>
      <arglist>(FVariant &amp;var)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>toLower</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>acc83ba4d9ccff6a513ed942b1449b783</anchor>
      <arglist>(uint32 index)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>toLower</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>adb7e069688aa84f251961a1e7714e355</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>toUpper</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a351fa9427bdbda2d4eeeec25347937e7</anchor>
      <arglist>(uint32 index)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>toUpper</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>aced434f03c0594b74bcd7453184b95a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>unsigned char *</type>
      <name>toPascalString</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a29369eb464b0594fbafba3c17ee8b5e4</anchor>
      <arglist>(unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>const String &amp;</type>
      <name>fromPascalString</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a95b58bddd98010222da389ed9548ad07</anchor>
      <arglist>(const unsigned char *buf)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>toWideString</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>ad140992bdbbaf77940afbf992d0b13df</anchor>
      <arglist>(uint32 sourceCodePage=kCP_Default)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>toMultiByte</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>acf18f38c6096b9f7fdff3089febf5e7e</anchor>
      <arglist>(uint32 destCodePage=kCP_Default)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>fromUTF8</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>abe9d4e30a36a330f0b75b6ea31bd0820</anchor>
      <arglist>(const char8 *utf8String)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>normalize</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a629eb76fcaf20205ab6c2482f85816eb</anchor>
      <arglist>(UnicodeNormalization=kUnicodeNormC)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>resize</name>
      <anchorfile>classSteinberg_1_1String.html</anchorfile>
      <anchor>a3f9845a9817eca153fc40325e9261318</anchor>
      <arglist>(uint32 newSize, bool wide, bool fill=false)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::StringObject</name>
    <filename>classSteinberg_1_1StringObject.html</filename>
    <base>Steinberg::FObject</base>
    <base>Steinberg::String</base>
    <member kind="function">
      <type></type>
      <name>StringObject</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>ac8b931dc2f0f7f581b341ee12747ef4b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>StringObject</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a4a8dd8b0035061f846ba7430b359753f</anchor>
      <arglist>(const char16 *str, int32 n=-1, bool isTerminated=true)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>StringObject</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a9496bb3a6cc76187b76608d07dab9f0a</anchor>
      <arglist>(const char8 *str, int32 n=-1, bool isTerminated=true)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>StringObject</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a343601d416df223b32a452756f4faf5c</anchor>
      <arglist>(const StringObject &amp;str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>StringObject</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a0cd28b016185969e68afc36f019ab680</anchor>
      <arglist>(const String &amp;str, int32 n=-1)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>StringObject</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a0dfc44527b214ea38efbc5e20b7bc3c7</anchor>
      <arglist>(const FVariant &amp;var)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setText</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>aeb29f2426e613590c2e115c359e36d2c</anchor>
      <arglist>(const char8 *text) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setText8</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>ab27dd84b242953e7a25e17d8a378744e</anchor>
      <arglist>(const char8 *text) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setText16</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>ab5045b0923620a934e187656a5890d80</anchor>
      <arglist>(const char16 *text) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>const char8 *</type>
      <name>getText8</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a37286d8e66991bc0b5324aac53fbfa1b</anchor>
      <arglist>() SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>const char16 *</type>
      <name>getText16</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>af3a5cb67454a0b6248915d69771d54a1</anchor>
      <arglist>() SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>take</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a2a3d39e91980f4df193d928554ab07ba</anchor>
      <arglist>(void *s, bool _isWide) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isWideString</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>abe0c5776749310c92457109ed51ce23a</anchor>
      <arglist>() const SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>OBJ_METHODS</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a6014978aa7395688bb87c6b744ec5ad5</anchor>
      <arglist>(StringObject, FObject) FUNKNOWN_METHODS2(IStringResult</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>ae2055bf8225e1ae1062abf2b2f63503c</anchor>
      <arglist>(const char8 *str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a9762fc85a717c44f1772dfd1933894b1</anchor>
      <arglist>(const char16 *str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a2d987a6d658b7e43497c9521ee809ea3</anchor>
      <arglist>(const ConstString &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>aa7694556e438408131523c949aeecdd0</anchor>
      <arglist>(const String &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a47163cdae5fe81ac503214ea71360686</anchor>
      <arglist>(char8 c)</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>operator=</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>a0b5b8173bdf45815f7ed64ae5a4235a2</anchor>
      <arglist>(char16 c)</arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>IString</name>
      <anchorfile>classSteinberg_1_1StringObject.html</anchorfile>
      <anchor>af9fefcb94e70d8d3410e0593c1fc7753</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::Timer</name>
    <filename>classSteinberg_1_1Timer.html</filename>
    <base>Steinberg::FObject</base>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>stop</name>
      <anchorfile>classSteinberg_1_1Timer.html</anchorfile>
      <anchor>a0efff8623a2fb79dad94a96dcf16d966</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Timer *</type>
      <name>create</name>
      <anchorfile>classSteinberg_1_1Timer.html</anchorfile>
      <anchor>a8bb2410bdbf225d3ed1bd5ead4a29f84</anchor>
      <arglist>(ITimerCallback *callback, uint32 intervalMilliseconds)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::UpdateHandler</name>
    <filename>classSteinberg_1_1UpdateHandler.html</filename>
    <base>Steinberg::FObject</base>
    <base>Steinberg::IUpdateManager</base>
    <member kind="function">
      <type></type>
      <name>UpdateHandler</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a5cb070dc5b6e125ca7dd0f609a266f0b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~UpdateHandler</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>ab3416815aefc3484de55b14892ae73dd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tresult</type>
      <name>addDependent</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a6525455e812688a1d9b5bcdb0c9fa77c</anchor>
      <arglist>(FUnknown *object, IDependent *dependent) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>tresult</type>
      <name>removeDependent</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a56b694b9f2c7fdd969fb7b95fd1c1d69</anchor>
      <arglist>(FUnknown *object, IDependent *dependent, size_t &amp;earseCount)</arglist>
    </member>
    <member kind="function">
      <type>tresult</type>
      <name>removeDependent</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a8d965c45a22cd44b2069b8d191d2d39d</anchor>
      <arglist>(FUnknown *object, IDependent *dependent) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>tresult</type>
      <name>triggerUpdates</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a0fc8d0ba8be89cecce9f6d82c804b59c</anchor>
      <arglist>(FUnknown *object, int32 message) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>tresult</type>
      <name>deferUpdates</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a1ba21e80d0caaaf9e1040c880d7e9927</anchor>
      <arglist>(FUnknown *object, int32 message) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>tresult</type>
      <name>cancelUpdates</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a0b58de7d5b5775a8ee4975abaf3e938e</anchor>
      <arglist>(FUnknown *object) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>tresult</type>
      <name>triggerDeferedUpdates</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>abf4b94fd02e2d92e6c335d3b6e6cb95f</anchor>
      <arglist>(FUnknown *object=nullptr) SMTG_OVERRIDE</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>countDependencies</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>acc7bf9a003c1026bdf6877fbbc48728a</anchor>
      <arglist>(FUnknown *object=nullptr)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>OBJ_METHODS</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>ada5cb0d0c48f04511582d6ac20eb4eb2</anchor>
      <arglist>(UpdateHandler, FObject) FUNKNOWN_METHODS2(IUpdateHandler</arglist>
    </member>
    <member kind="function">
      <type>virtual void</type>
      <name>addDependent</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>aad64a1f592d31c920bc72505ab860576</anchor>
      <arglist>(IDependent *dep)</arglist>
    </member>
    <member kind="function">
      <type>virtual void</type>
      <name>removeDependent</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a4a343976011506b85607300a4bd97832</anchor>
      <arglist>(IDependent *dep)</arglist>
    </member>
    <member kind="function">
      <type>virtual void</type>
      <name>deferUpdate</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>ad0cb02215c1c6e44b53f177e7844a9f5</anchor>
      <arglist>(int32 msg=kChanged)</arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>IUpdateManager</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>aa1cef82bd3c810132b0b17f553465564</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>FObject SINGLETON(UpdateHandler) private Steinberg::Base::Thread::FLock</type>
      <name>lock</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a10f007ffba0c693f361ca6251345a84f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Update::Table *</type>
      <name>table</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>a11ab8afebbe1338f626a6d1be0137c4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>FObject</name>
      <anchorfile>classSteinberg_1_1UpdateHandler.html</anchorfile>
      <anchor>acb477dd25b6cacad13521cf9401dda3f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Steinberg::CommandLine::VariablesMap</name>
    <filename>classSteinberg_1_1CommandLine_1_1VariablesMap.html</filename>
    <member kind="function">
      <type></type>
      <name>VariablesMap</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1VariablesMap.html</anchorfile>
      <anchor>abd055542f549cbbfbf227f22f49f3295</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hasError</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1VariablesMap.html</anchorfile>
      <anchor>a81dd125e253592aaef5fea33dfc50c42</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setError</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1VariablesMap.html</anchorfile>
      <anchor>a4f9dcc805975eb9dcf2579cd3886091e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::string &amp;</type>
      <name>operator []</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1VariablesMap.html</anchorfile>
      <anchor>a9762d41d3885a10785a931b9e32404a9</anchor>
      <arglist>(const VariablesMapContainer::key_type k)</arglist>
    </member>
    <member kind="function">
      <type>const std::string &amp;</type>
      <name>operator []</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1VariablesMap.html</anchorfile>
      <anchor>a7d4195bb89e09cb3731cf589e3ae63f3</anchor>
      <arglist>(const VariablesMapContainer::key_type k) const</arglist>
    </member>
    <member kind="function">
      <type>VariablesMapContainer::size_type</type>
      <name>count</name>
      <anchorfile>classSteinberg_1_1CommandLine_1_1VariablesMap.html</anchorfile>
      <anchor>aae1a1de067fae18405ef48dfbe0b3c00</anchor>
      <arglist>(const VariablesMapContainer::key_type k) const</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>Steinberg</name>
    <filename>namespaceSteinberg.html</filename>
    <namespace>Steinberg::CommandLine</namespace>
    <namespace>Steinberg::HexBinary</namespace>
    <namespace>Steinberg::Singleton</namespace>
    <namespace>Steinberg::SystemTime</namespace>
    <class kind="class">Steinberg::Buffer</class>
    <class kind="class">Steinberg::ConstString</class>
    <class kind="class">Steinberg::DisableDispatchingTimers</class>
    <class kind="struct">Steinberg::FArrayDeleter</class>
    <class kind="struct">Steinberg::FBooleanSetter</class>
    <class kind="struct">Steinberg::FCloser</class>
    <class kind="struct">Steinberg::FConditionalBoolSetter</class>
    <class kind="struct">Steinberg::FDeleter</class>
    <class kind="class">Steinberg::FDynLibrary</class>
    <class kind="class">Steinberg::FMallocReleaser</class>
    <class kind="struct">Steinberg::FNuller</class>
    <class kind="class">Steinberg::FObject</class>
    <class kind="struct">Steinberg::FPtrNuller</class>
    <class kind="class">Steinberg::FStreamer</class>
    <class kind="class">Steinberg::FStreamSizeHolder</class>
    <class kind="class">Steinberg::IBStreamer</class>
    <class kind="class">Steinberg::ITimerCallback</class>
    <class kind="class">Steinberg::IUpdateManager</class>
    <class kind="class">Steinberg::String</class>
    <class kind="class">Steinberg::StringObject</class>
    <class kind="class">Steinberg::Timer</class>
    <class kind="class">Steinberg::UpdateHandler</class>
    <member kind="typedef">
      <type>FBooleanSetter&lt; bool &gt;</type>
      <name>FBoolSetter</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>aac248a0d0bf4446d45f2539bc61f6c23</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>FIDString</type>
      <name>FClassID</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab36f267af0b3dc88ce9135a0c3b4f170</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>FSeekMode</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a78e5e10c2df97a6a5a6096069ade24db</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSeekSet</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a78e5e10c2df97a6a5a6096069ade24dbad9fe3df36943c836f26fd55f2e800384</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSeekCurrent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a78e5e10c2df97a6a5a6096069ade24dbaff1a9d5b4238462a49619084e8324b85</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kSeekEnd</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a78e5e10c2df97a6a5a6096069ade24dbaa2d1805ccbf869dc4a610f9571d83d7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MBCodePage</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_ANSI</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198a4bc87ed163042e19a3c8c857f320e601</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_MAC_ROMAN</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198aece485d20e1f11025207f5d03331170b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_ANSI_WEL</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198ae306671b758491ebea5651afc41faf06</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_MAC_CEE</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198afdfb28513dca7897ba46509177f3f295</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_Utf8</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198a42fe778326afe1a478de66fa0ae88053</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_ShiftJIS</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198a800fe314d21309f85659ee3c8ff41fba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_US_ASCII</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198a68f73e16ffd280be8ea415937c17901e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kCP_Default</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a879b9da0475eafb05527d00a3e87e198ab93a845dd7dd03d5e296ffff03a78aa2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>UnicodeNormalization</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kUnicodeNormC</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5a5ab93cdbd8bb16cc7261b7d02f656426</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kUnicodeNormD</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5a719813623d8e0a706e6ab6cd2a0fb0f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kUnicodeNormKC</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5a6bc71341f3a88c030bc7f46992d39c99</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>kUnicodeNormKD</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ade9b236510365125beb73ce8c7347fd5ad8daa621694a792a6b4368b2e1caccad</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>C *</type>
      <name>FCast</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7eedfe7a9261c91c5c4725c762d0302d</anchor>
      <arglist>(const FObject *object)</arglist>
    </member>
    <member kind="function">
      <type>C *</type>
      <name>FCast</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a64e77e379512698cdbed2c00f9a6dd38</anchor>
      <arglist>(FUnknown *unknown)</arglist>
    </member>
    <member kind="function">
      <type>C *</type>
      <name>FUCast</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a0f07be811035c79903a4239709c274f3</anchor>
      <arglist>(FObject *object)</arglist>
    </member>
    <member kind="function">
      <type>C *</type>
      <name>FUCast</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>aec0fb92a1a224a4dc1a4e323505e52d5</anchor>
      <arglist>(FUnknown *object)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>AssignShared</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a061c8d1b219f1b80adf65d2c1d36e899</anchor>
      <arglist>(T *&amp;dest, T *newPtr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>AssignSharedDependent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a52db63df2d44c7a493bf7b5ce39df1a6</anchor>
      <arglist>(IDependent *_this, T *&amp;dest, T *newPtr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>AssignSharedDependent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a8567982a150047c011e89c7185871d96</anchor>
      <arglist>(IDependent *_this, IPtr&lt; T &gt; &amp;dest, T *newPtr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeReleaseDependent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a0869140d4f11c3f1dae20794daa85b5f</anchor>
      <arglist>(IDependent *_this, T *&amp;dest)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeReleaseDependent</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ad047921ea5c9ffd8f4b5e2c31d30d819</anchor>
      <arglist>(IDependent *_this, IPtr&lt; T &gt; &amp;dest)</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hashString8</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a166ebc6677e19253cda1f55ead2b8106</anchor>
      <arglist>(const char8 *s, uint32 m)</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hashString16</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a61e26dc8165fa82a90f930a1f8367a3c</anchor>
      <arglist>(const char16 *s, uint32 m)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>tstrnatcmp</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>adcccbbf0a6580038c4fb1f0dbf91667f</anchor>
      <arglist>(const T *s1, const T *s2, bool caseSensitive=true)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>strnatcmp8</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab16cda4272acda12af7fe328eb3abbfa</anchor>
      <arglist>(const char8 *s1, const char8 *s2, bool caseSensitive)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>strnatcmp16</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a026eef900ab2e9f3c1390d73992a48a2</anchor>
      <arglist>(const char16 *s1, const char16 *s2, bool caseSensitive)</arglist>
    </member>
    <member kind="function">
      <type>uint32</type>
      <name>hashString</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab9e1f71a717bf28978e67938cf197360</anchor>
      <arglist>(const char *s, uint32 m)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ada25fd2a2681fec14af4507826ffdfe8</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a2c3adccbf0361ed1e35dc58f474aa783</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a0047a2ab78ce4594023b5691eda91128</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>adafb26f233ad745fb7503011697a8d89</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a4d79863da188e183ae4150116906ad6b</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a1a02a54515930b11df9cb7c05ed0a663</anchor>
      <arglist>(const ConstString &amp;s1, const String &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af032c74c6993e1abfbd7bccda3fea33d</anchor>
      <arglist>(const String &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ac18cc72253d13ec535ba0187d07759af</anchor>
      <arglist>(const String &amp;s1, const String &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>aa19edede938efc8875df07e2079cb898</anchor>
      <arglist>(const String &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ae356517d1995baa9053ae4cbec848b85</anchor>
      <arglist>(const String &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a86f7c6c4e0f84a11e5583452e809debe</anchor>
      <arglist>(const char8 *s1, const String &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>operator+</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a484a5c7b2980c3eddde1777f4f8740e2</anchor>
      <arglist>(const char16 *s1, const String &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ad5ce23c4f9cca43a9f66ef23d9325159</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>afc04c8ac4717595b641a119f8e1f4c08</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ac98deb5a1f36329c44edeee647d46431</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a98082cddf36f42dae42928aab7b7d704</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af65b89110f91b435a658a09cde45ee6b</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>aaad28d7939d8454ba62e2ed43e6b6a07</anchor>
      <arglist>(const ConstString &amp;s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>acf5c2d96088a26678de1f7ba2b0610a9</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab4e3d70dc9e542aa4830d9026f49e318</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a296a0841f7c8f2fc3a5708556c720a5a</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a111fe3210982295b10c2b0a6c64f8849</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a8e6d100fac4f2e9011a595eb516d21ea</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>acf534ee8bc3f7368a3a21e666eb87e88</anchor>
      <arglist>(const ConstString &amp;s1, const char8 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7e81e3384f1e73b68dba43f8ca561b27</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7784e17f867a990478e8b3a25216f047</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7792b96abbdb52f88644a2d7fdc0635d</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a763dcfcbf62f158517e3265575e09b03</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af532a1fe0c1fa0b787fb1c6fa58048e4</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a294195888b30e4aa51ac89171313dddd</anchor>
      <arglist>(const char8 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a7a3ef53c29d18481261d4ad40fbd93d8</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a5dac4595f7ec9120e736fc4a72b263a4</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a59823787882c26e6dc2a849a23785b22</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a1bfad3a77225793f845ad097c4b694a6</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a5998cbea9b1218b35018328826e6a40d</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a34e7201686726b43099edeabc6e64b02</anchor>
      <arglist>(const ConstString &amp;s1, const char16 *s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a4a5e176ffe4e3d42b9dbdd5ff6b6a232</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>ab43ab059cf7a1b2eee6dcf7db9429f72</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af8868f64b08e62fc4ca89095b748dcb3</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a70fa44d88af75be3d8d5402aa761370c</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a645271d1621265af41557ff516b67961</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a262d596d26d40f41fab24af7162c7020</anchor>
      <arglist>(const char16 *s1, const ConstString &amp;s2)</arglist>
    </member>
    <member kind="function">
      <type>int32</type>
      <name>strnatcmp</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af813b691586a7d0180f9275b88d8ce86</anchor>
      <arglist>(const char *s1, const char *s2, bool caseSensitive=true)</arglist>
    </member>
    <member kind="function">
      <type>ITimerCallback *</type>
      <name>newTimerCallback</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>af754042087ab79da3c3b0d08681e9a93</anchor>
      <arglist>(const Call &amp;call)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DEF_CLASS_IID</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a03374ef5cb2e4ca5d36313f4c3dd85e2</anchor>
      <arglist>(IUpdateManager) namespace Update</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeRelease</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a694fec3dd0db8909ea3289ba1de4f494</anchor>
      <arglist>(I *&amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeRelease</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a2b4982585b3f7bc20622a0f5b81fcb43</anchor>
      <arglist>(IPtr&lt; I &gt; &amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeDelete</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a1a7b3d6a54e6b821d843acdbb90ec7a1</anchor>
      <arglist>(T *&amp;ptr)</arglist>
    </member>
    <member kind="variable">
      <type>struct Steinberg::FObjectIIDInitializer</type>
      <name>gFObjectIidInitializer</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a716e8faa9615ce0d3a3b357275d14868</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeRelease</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a694fec3dd0db8909ea3289ba1de4f494</anchor>
      <arglist>(I *&amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeRelease</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a2b4982585b3f7bc20622a0f5b81fcb43</anchor>
      <arglist>(IPtr&lt; I &gt; &amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SafeDelete</name>
      <anchorfile>namespaceSteinberg.html</anchorfile>
      <anchor>a1a7b3d6a54e6b821d843acdbb90ec7a1</anchor>
      <arglist>(T *&amp;ptr)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>Steinberg::CommandLine</name>
    <filename>namespaceSteinberg_1_1CommandLine.html</filename>
    <class kind="class">Steinberg::CommandLine::Description</class>
    <class kind="class">Steinberg::CommandLine::Descriptions</class>
    <class kind="class">Steinberg::CommandLine::VariablesMap</class>
    <member kind="typedef">
      <type>std::vector&lt; std::string &gt;</type>
      <name>FilesVector</name>
      <anchorfile>namespaceSteinberg_1_1CommandLine.html</anchorfile>
      <anchor>a8e4c7d85763c5d6cafd67588804308f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>parse</name>
      <anchorfile>namespaceSteinberg_1_1CommandLine.html</anchorfile>
      <anchor>aa59540066a0354da873325a175666974</anchor>
      <arglist>(int ac, char *av[], const Descriptions &amp;desc, VariablesMap &amp;result, FilesVector *files=nullptr)</arglist>
    </member>
    <member kind="function">
      <type>std::ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>namespaceSteinberg_1_1CommandLine.html</anchorfile>
      <anchor>a6a38965238a994bf87587aac0b83a5be</anchor>
      <arglist>(std::ostream &amp;os, const Descriptions &amp;desc)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>Steinberg::HexBinary</name>
    <filename>namespaceSteinberg_1_1HexBinary.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>decode</name>
      <anchorfile>namespaceSteinberg_1_1HexBinary.html</anchorfile>
      <anchor>a567d62615b1d9ccab3b175b4b9fc61f0</anchor>
      <arglist>(const void *input, int32 inputSize, Buffer &amp;result)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>encode</name>
      <anchorfile>namespaceSteinberg_1_1HexBinary.html</anchorfile>
      <anchor>a409cabd0268ae39045fe68f7816a06af</anchor>
      <arglist>(const void *input, int32 inputSize, Buffer &amp;result)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>Steinberg::Singleton</name>
    <filename>namespaceSteinberg_1_1Singleton.html</filename>
    <member kind="typedef">
      <type>std::vector&lt; FObject ** &gt;</type>
      <name>ObjectVector</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ae71bce85324f37d598620c2adc38733f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isTerminated</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a84acfabb69fd8f78cc6afad4301357b7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lockRegister</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a1612cf2b7b00489ee4cd0af7999ff7ac</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unlockRegister</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>a2aad80bf0469cb9153a7666f2b2b4817</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>registerInstance</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ad6e6c857d3abf9d8ef37ef8d465c9977</anchor>
      <arglist>(FObject **o)</arglist>
    </member>
    <member kind="variable">
      <type>ObjectVector *</type>
      <name>singletonInstances</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ab5cb5c38a31d778cf922b01947589311</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>singletonsTerminated</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ae52f18a708ab305abe54c48ffc1c9840</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Steinberg::Base::Thread::FLock *</type>
      <name>singletonsLock</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ac67df8722321fe3f07d688a9710006cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct Steinberg::Singleton::Deleter</type>
      <name>deleter</name>
      <anchorfile>namespaceSteinberg_1_1Singleton.html</anchorfile>
      <anchor>ad6199b16f89340fdc59b661207d8ae40</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>Steinberg::SystemTime</name>
    <filename>namespaceSteinberg_1_1SystemTime.html</filename>
    <member kind="function">
      <type>int32</type>
      <name>getTicks</name>
      <anchorfile>namespaceSteinberg_1_1SystemTime.html</anchorfile>
      <anchor>ae994ceba61021313d4200996d46787f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint64</type>
      <name>getTicks64</name>
      <anchorfile>namespaceSteinberg_1_1SystemTime.html</anchorfile>
      <anchor>accc64954e88f0a32add172d0189918dc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uint64</type>
      <name>getTicksDuration</name>
      <anchorfile>namespaceSteinberg_1_1SystemTime.html</anchorfile>
      <anchor>a5eb6e497830d117c9591a9d18faa3816</anchor>
      <arglist>(uint64 old, uint64 now)</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Base Module</title>
    <filename>index</filename>
    <docanchor file="index" title="Introduction">baseIntro</docanchor>
    <docanchor file="index" title="Strings">baseString</docanchor>
    <docanchor file="index" title="IPtr Smart Pointer">baseIPtr</docanchor>
    <docanchor file="index" title="Steinberg::FObject">baseFObject</docanchor>
    <docanchor file="index" title="Dependencies">baseDependencies</docanchor>
    <docanchor file="index" title="Debugging">basefdebug_h</docanchor>
    <docanchor file="index" title="Also useful">baseOtherUsefulClasses</docanchor>
  </compound>
</tagfile>
