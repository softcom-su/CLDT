package su.softcom.cldt.ui.resolvers.quickfixes;

import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

/**
 * A class with all clang-tidy check names, stores the checks that support the
 * fix separately.
 */
public class ClangTidyChecks {

	private static final Set<String> ALL_CHECKS = new HashSet<>();

	private static final Set<String> CHECKS_WITHOUT_FIXES = new HashSet<>();

	private static final Set<String> AUTO_FIXABLE_CHECKS = new HashSet<>();
	private static final Set<String> RENAME_FIXABLE_CHECKS = new HashSet<>();

	private ClangTidyChecks() {
	}

	static {
		// Abseil
		addAutoFixableChecks("abseil-cleanup-ctad", "abseil-duration-addition", "abseil-duration-comparison", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"abseil-duration-conversion-cast", "abseil-duration-division", "abseil-duration-factory-float", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"abseil-duration-factory-scale", "abseil-duration-subtraction", //$NON-NLS-1$ //$NON-NLS-2$
				"abseil-duration-unnecessary-conversion", "abseil-faster-strsplit-delimiter", //$NON-NLS-1$ //$NON-NLS-2$
				"abseil-redundant-strcat-calls", "abseil-str-cat-append", "abseil-string-find-startswith", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"abseil-string-find-str-contains", "abseil-time-comparison", "abseil-time-subtraction", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"abseil-upgrade-duration-conversions"); //$NON-NLS-1$

		// Altera
		addAutoFixableChecks("altera-struct-pack-align"); //$NON-NLS-1$

		// Android
		addAutoFixableChecks("android-cloexec-accept", "android-cloexec-accept4", "android-cloexec-creat", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"android-cloexec-dup", "android-cloexec-epoll-create", "android-cloexec-epoll-create1", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"android-cloexec-fopen", "android-cloexec-inotify-init", "android-cloexec-inotify-init1", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"android-cloexec-memfd-create", "android-cloexec-open", "android-cloexec-pipe", "android-cloexec-pipe2", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
				"android-cloexec-socket"); //$NON-NLS-1$

		// Boost
		addAutoFixableChecks("boost-use-ranges", "boost-use-to-string"); //$NON-NLS-1$ //$NON-NLS-2$

		// Bugprone
		addAutoFixableChecks("bugprone-argument-comment", "bugprone-bool-pointer-implicit-conversion", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-copy-constructor-init", "bugprone-crtp-constructor-accessibility", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-implicit-widening-of-multiplication-result", "bugprone-inaccurate-erase", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-incorrect-enable-if", "bugprone-incorrect-enable-shared-from-this", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-macro-parentheses", "bugprone-misplaced-operator-in-strlen-in-alloc", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-misplaced-pointer-arithmetic-in-alloc", "bugprone-move-forwarding-reference", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-not-null-terminated-result", "bugprone-optional-value-conversion", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-parent-virtual-call", "bugprone-posix-return", "bugprone-redundant-branch-condition", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-reserved-identifier", "bugprone-shared-ptr-array-mismatch", "bugprone-standalone-empty", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-string-constructor", "bugprone-string-integer-assignment", "bugprone-stringview-nullptr", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-suspicious-memset-usage", "bugprone-suspicious-semicolon", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-suspicious-string-compare", "bugprone-swapped-arguments", "bugprone-terminating-continue", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-unintended-char-ostream-output", "bugprone-unique-ptr-array-mismatch", "bugprone-unused-raii", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-virtual-near-miss"); //$NON-NLS-1$

		// CERT
		addAutoFixableChecks("cert-dcl03-c", "cert-dcl16-c", "cert-dcl37-c", "cert-dcl51-cpp", "cert-int09-c"); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$

		// CppCoreGuidelines
		addAutoFixableChecks("cppcoreguidelines-explicit-virtual-functions", "cppcoreguidelines-init-variables", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-macro-to-enum", "cppcoreguidelines-misleading-capture-default-by-value", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-noexcept-destructor", "cppcoreguidelines-noexcept-move-operations", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-noexcept-swap", "cppcoreguidelines-prefer-member-initializer", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-pro-bounds-constant-array-index", "cppcoreguidelines-pro-type-cstyle-cast", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-pro-type-member-init", "cppcoreguidelines-pro-type-static-cast-downcast", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-use-default-member-init", "cppcoreguidelines-virtual-class-destructor"); //$NON-NLS-1$ //$NON-NLS-2$

		// Darwin
		addAutoFixableChecks("darwin-dispatch-once-nonstatic"); //$NON-NLS-1$

		// Fuchsia
		addAutoFixableChecks("fuchsia-default-arguments-declarations"); //$NON-NLS-1$

		// Google
		addAutoFixableChecks("google-explicit-constructor", "google-upgrade-googletest-case"); //$NON-NLS-1$ //$NON-NLS-2$

		// Hicpp
		addAutoFixableChecks("hicpp-deprecated-headers", "hicpp-explicit-conversions", "hicpp-member-init", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"hicpp-move-const-arg", "hicpp-named-parameter", "hicpp-noexcept-move", "hicpp-static-assert", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
				"hicpp-uppercase-literal-suffix", "hicpp-use-auto", "hicpp-use-emplace", "hicpp-use-equals-default", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
				"hicpp-use-equals-delete", "hicpp-use-noexcept", "hicpp-use-nullptr", "hicpp-use-override"); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$

		// LLVM
		addAutoFixableChecks("llvm-else-after-return", "llvm-include-order", //$NON-NLS-1$ //$NON-NLS-2$
				"llvm-prefer-isa-or-dyn-cast-in-conditionals", "llvm-prefer-register-over-unsigned", //$NON-NLS-1$ //$NON-NLS-2$
				"llvm-qualified-auto", "llvm-twine-local"); //$NON-NLS-1$ //$NON-NLS-2$

		// LLVMLibc
		addAutoFixableChecks("llvmlibc-inline-function-decl", "llvmlibc-restrict-system-libc-headers"); //$NON-NLS-1$ //$NON-NLS-2$

		// Misc
		addAutoFixableChecks("misc-const-correctness", "misc-definitions-in-headers", "misc-include-cleaner", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"misc-redundant-expression", "misc-static-assert", "misc-uniqueptr-reset-release", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"misc-unused-alias-decls", "misc-unused-parameters", "misc-unused-using-decls", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"misc-use-internal-linkage"); //$NON-NLS-1$

		// Modernize
		addAutoFixableChecks("modernize-avoid-bind", "modernize-concat-nested-namespaces", //$NON-NLS-1$ //$NON-NLS-2$
				"modernize-deprecated-headers", "modernize-deprecated-ios-base-aliases", "modernize-loop-convert", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-macro-to-enum", "modernize-make-shared", "modernize-make-unique", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-min-max-use-initializer-list", "modernize-pass-by-value", "modernize-raw-string-literal", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-redundant-void-arg", "modernize-replace-auto-ptr", //$NON-NLS-1$ //$NON-NLS-2$
				"modernize-replace-disallow-copy-and-assign-macro", "modernize-replace-random-shuffle", //$NON-NLS-1$ //$NON-NLS-2$
				"modernize-return-braced-init-list", "modernize-shrink-to-fit", "modernize-type-traits", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-unary-static-assert", "modernize-use-auto", "modernize-use-bool-literals", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-use-constraints", "modernize-use-default-member-init", //$NON-NLS-1$ //$NON-NLS-2$
				"modernize-use-designated-initializers", "modernize-use-emplace", "modernize-use-equals-default", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-use-equals-delete", "modernize-use-integer-sign-comparison", "modernize-use-nodiscard", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-use-noexcept", "modernize-use-nullptr", "modernize-use-override", "modernize-use-ranges", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
				"modernize-use-starts-ends-with", "modernize-use-std-format", "modernize-use-std-numbers", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-use-std-print", "modernize-use-trailing-return-type", "modernize-use-transparent-functors", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"modernize-use-uncaught-exceptions", "modernize-use-using"); //$NON-NLS-1$ //$NON-NLS-2$

		// Mpi
		addAutoFixableChecks("mpi-buffer-deref", "mpi-type-mismatch"); //$NON-NLS-1$ //$NON-NLS-2$

		// Objc
		addAutoFixableChecks("objc-assert-equals", "objc-nsinvocation-argument-lifetime", "objc-property-declaration", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"objc-super-self"); //$NON-NLS-1$

		// Performance
		addAutoFixableChecks("performance-avoid-endl", "performance-faster-string-find", "performance-for-range-copy", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"performance-inefficient-algorithm", "performance-inefficient-vector-operation", //$NON-NLS-1$ //$NON-NLS-2$
				"performance-move-const-arg", "performance-noexcept-destructor", //$NON-NLS-1$ //$NON-NLS-2$
				"performance-noexcept-move-constructor", "performance-noexcept-swap", //$NON-NLS-1$ //$NON-NLS-2$
				"performance-trivially-destructible", "performance-type-promotion-in-math-fn", //$NON-NLS-1$ //$NON-NLS-2$
				"performance-unnecessary-copy-initialization", "performance-unnecessary-value-param"); //$NON-NLS-1$ //$NON-NLS-2$

		// Portability
		addAutoFixableChecks("portability-restrict-system-includes"); //$NON-NLS-1$

		// Readability
		addAutoFixableChecks("readability-ambiguous-smartptr-reset-call", "readability-avoid-const-params-in-decls", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-avoid-return-with-void-value", "readability-const-return-type", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-container-contains", "readability-container-data-pointer", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-container-size-empty", "readability-convert-member-functions-to-static", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-delete-null-pointer", "readability-duplicate-include", "readability-else-after-return", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"readability-enum-initial-value", "readability-identifier-naming", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-implicit-bool-conversion", "readability-inconsistent-declaration-parameter-name", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-isolate-declaration", "readability-make-member-function-const", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-math-missing-parentheses", "readability-misplaced-array-index", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-named-parameter", "readability-non-const-parameter", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-operators-representation", "readability-qualified-auto", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-redundant-access-specifiers", "readability-redundant-casting", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-redundant-control-flow", "readability-redundant-declaration", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-redundant-function-ptr-dereference", "readability-redundant-inline-specifier", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-redundant-member-init", "readability-redundant-smartptr-get", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-redundant-string-cstr", "readability-redundant-string-init", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-simplify-boolean-expr", "readability-simplify-subscript-expr", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-static-accessed-through-instance", "readability-static-definition-in-anonymous-namespace", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-string-compare", "readability-uniqueptr-delete-release", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-uppercase-literal-suffix", "readability-use-std-min-max"); //$NON-NLS-1$ //$NON-NLS-2$

		// Rename checks
		addRenameFixableChecks("readability-identifier-length", "misc-confusable-identifiers"); //$NON-NLS-1$ //$NON-NLS-2$
	}

	// Adding checks without fixes
	static {
		// abseil-*
		addChecks("abseil-no-internal-dependencies", "abseil-no-namespace"); //$NON-NLS-1$ //$NON-NLS-2$

		// altera-*
		addChecks("altera-id-dependent-backward-branch", "altera-kernel-name-restriction", //$NON-NLS-1$ //$NON-NLS-2$
				"altera-single-work-item-barrier", "altera-unroll-loops"); //$NON-NLS-1$ //$NON-NLS-2$

		// android-*
		addChecks("android-comparison-in-temp-failure-retry"); //$NON-NLS-1$

		// bugprone-*
		addChecks("bugprone-assert-side-effect", "bugprone-assignment-in-if-condition", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-bad-signal-to-kill-thread", "bugprone-bitwise-pointer-cast", "bugprone-branch-clone", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-capturing-this-in-member-variable", "bugprone-casting-through-void", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-chained-comparison", "bugprone-compare-pointer-to-member-virtual-function", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-dangling-handle", "bugprone-dynamic-static-initializers", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-easily-swappable-parameters", "bugprone-empty-catch", "bugprone-exception-escape", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-fold-init-type", "bugprone-forward-declaration-namespace", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-forwarding-reference-overload", "bugprone-inc-dec-in-conditions", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-incorrect-roundings", "bugprone-infinite-loop", "bugprone-integer-division", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-lambda-function-name", "bugprone-macro-repeated-side-effects", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-misleading-setter-of-reference", "bugprone-misplaced-widening-cast", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-multi-level-implicit-pointer-conversion", "bugprone-multiple-new-in-one-expression", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-multiple-statement-macro", "bugprone-narrowing-conversions", "bugprone-no-escape", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-non-zero-enum-to-bool-conversion", "bugprone-nondeterministic-pointer-iteration-order", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-pointer-arithmetic-on-polymorphic-object", "bugprone-return-const-ref-from-parameter", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-signal-handler", "bugprone-signed-char-misuse", "bugprone-sizeof-container", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"bugprone-sizeof-expression", "bugprone-spuriously-wake-up-functions", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-string-literal-with-embedded-nul", "bugprone-suspicious-enum-usage", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-suspicious-include", "bugprone-suspicious-memory-comparison", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-suspicious-missing-comma", "bugprone-suspicious-realloc-usage", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-suspicious-stringview-data-usage", "bugprone-switch-missing-default-case", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-tagged-union-member-count", "bugprone-throw-keyword-missing", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-too-small-loop-variable", "bugprone-unchecked-optional-access", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-undefined-memory-manipulation", "bugprone-undelegated-constructor", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-unhandled-exception-at-new", "bugprone-unhandled-self-assignment", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-unsafe-functions", "bugprone-unused-local-non-trivial-variable", //$NON-NLS-1$ //$NON-NLS-2$
				"bugprone-unused-return-value", "bugprone-use-after-move"); //$NON-NLS-1$ //$NON-NLS-2$

		// cert-*
		addChecks("cert-arr39-c", "cert-con36-c", "cert-con54-cpp", "cert-ctr56-cpp", "cert-dcl50-cpp", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$
				"cert-dcl54-cpp", "cert-dcl58-cpp", "cert-dcl59-cpp", "cert-env33-c", "cert-err09-cpp", "cert-err33-c", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$
				"cert-err34-c", "cert-err52-cpp", "cert-err58-cpp", "cert-err60-cpp", "cert-err61-cpp", "cert-exp42-c", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$
				"cert-fio38-c", "cert-flp30-c", "cert-flp37-c", "cert-mem57-cpp", "cert-msc24-c", "cert-msc30-c", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$
				"cert-msc32-c", "cert-msc33-c", "cert-msc50-cpp", "cert-msc51-cpp", "cert-msc54-cpp", "cert-oop11-cpp", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$
				"cert-oop54-cpp", "cert-oop57-cpp", "cert-oop58-cpp", "cert-pos44-c", "cert-pos47-c", "cert-sig30-c", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$
				"cert-str34-c"); //$NON-NLS-1$

		// concurrency-*
		addChecks("concurrency-mt-unsafe", "concurrency-thread-canceltype-asynchronous"); //$NON-NLS-1$ //$NON-NLS-2$

		// cppcoreguidelines-*
		addChecks("cppcoreguidelines-avoid-c-arrays", "cppcoreguidelines-avoid-capturing-lambda-coroutines", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-avoid-const-or-ref-data-members", "cppcoreguidelines-avoid-do-while", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-avoid-goto", "cppcoreguidelines-avoid-magic-numbers", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-avoid-non-const-global-variables", //$NON-NLS-1$
				"cppcoreguidelines-avoid-reference-coroutine-parameters", //$NON-NLS-1$
				"cppcoreguidelines-c-copy-assignment-signature", "cppcoreguidelines-interfaces-global-init", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-macro-usage", "cppcoreguidelines-missing-std-forward", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-narrowing-conversions", "cppcoreguidelines-no-malloc", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-no-suspend-with-lock", "cppcoreguidelines-non-private-member-variables-in-classes", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-owning-memory", "cppcoreguidelines-pro-bounds-array-to-pointer-decay", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-pro-bounds-pointer-arithmetic", "cppcoreguidelines-pro-type-const-cast", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-pro-type-reinterpret-cast", "cppcoreguidelines-pro-type-union-access", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-pro-type-vararg", "cppcoreguidelines-rvalue-reference-param-not-moved", //$NON-NLS-1$ //$NON-NLS-2$
				"cppcoreguidelines-slicing", "cppcoreguidelines-special-member-functions"); //$NON-NLS-1$ //$NON-NLS-2$

		// darwin-*
		addChecks("darwin-avoid-spinlock"); //$NON-NLS-1$

		// fuchsia-*
		addChecks("fuchsia-default-arguments-calls", "fuchsia-header-anon-namespaces", "fuchsia-multiple-inheritance", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"fuchsia-overloaded-operator", "fuchsia-statically-constructed-objects", "fuchsia-trailing-return", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"fuchsia-virtual-inheritance"); //$NON-NLS-1$

		// google-*
		addChecks("google-build-explicit-make-pair", "google-build-namespaces", "google-build-using-namespace", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"google-default-arguments", "google-global-names-in-headers", "google-objc-avoid-nsobject-new", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"google-objc-avoid-throwing-exception", "google-objc-function-naming", //$NON-NLS-1$ //$NON-NLS-2$
				"google-objc-global-variable-declaration", "google-readability-avoid-underscore-in-googletest-name", //$NON-NLS-1$ //$NON-NLS-2$
				"google-readability-braces-around-statements", "google-readability-casting", //$NON-NLS-1$ //$NON-NLS-2$
				"google-readability-function-size", "google-readability-namespace-comments", "google-readability-todo", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"google-runtime-int", "google-runtime-operator"); //$NON-NLS-1$ //$NON-NLS-2$

		// hicpp-*
		addChecks("hicpp-avoid-c-arrays", "hicpp-avoid-goto", "hicpp-braces-around-statements", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"hicpp-exception-baseclass", "hicpp-function-size", "hicpp-ignored-remove-result", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"hicpp-invalid-access-moved", "hicpp-multiway-paths-covered", "hicpp-new-delete-operators", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"hicpp-no-array-decay", "hicpp-no-assembler", "hicpp-no-malloc", "hicpp-signed-bitwise", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
				"hicpp-special-member-functions", "hicpp-undelegated-constructor", "hicpp-vararg"); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$

		// linuxkernel-*
		addChecks("linuxkernel-must-check-errs"); //$NON-NLS-1$

		// llvm-*
		addChecks("llvm-header-guard", "llvm-namespace-comment"); //$NON-NLS-1$ //$NON-NLS-2$

		// llvmlibc-*
		addChecks("llvmlibc-callee-namespace", "llvmlibc-implementation-in-namespace"); //$NON-NLS-1$ //$NON-NLS-2$

		// misc-*
		addChecks("misc-confusable-identifiers", "misc-coroutine-hostile-raii", "misc-header-include-cycle", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"misc-misleading-bidirectional", "misc-misleading-identifier", "misc-misplaced-const", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"misc-new-delete-overloads", "misc-no-recursion", "misc-non-copyable-objects", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"misc-non-private-member-variables-in-classes", "misc-throw-by-value-catch-by-reference", //$NON-NLS-1$ //$NON-NLS-2$
				"misc-unconventional-assign-operator", "misc-use-anonymous-namespace"); //$NON-NLS-1$ //$NON-NLS-2$

		// modernize-*
		addChecks("modernize-avoid-c-arrays"); //$NON-NLS-1$

		// objc-*
		addChecks("objc-avoid-nserror-init", "objc-dealloc-in-category", "objc-forbidden-subclassing", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"objc-missing-hash", "objc-nsdate-formatter"); //$NON-NLS-1$ //$NON-NLS-2$

		// openmp-*
		addChecks("openmp-exception-escape", "openmp-use-default-none"); //$NON-NLS-1$ //$NON-NLS-2$

		// performance-*
		addChecks("performance-enum-size", "performance-implicit-conversion-in-loop", //$NON-NLS-1$ //$NON-NLS-2$
				"performance-inefficient-string-concatenation", "performance-move-constructor-init", //$NON-NLS-1$//$NON-NLS-2$
				"performance-no-automatic-move", "performance-no-int-to-ptr"); //$NON-NLS-1$ //$NON-NLS-2$

		// portability-*
		addChecks("portability-avoid-pragma-once", "portability-simd-intrinsics", "portability-std-allocator-const", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"portability-template-virtual-member-function"); //$NON-NLS-1$

		// readability-*
		addChecks("readability-avoid-nested-conditional-operator", "readability-avoid-unconditional-preprocessor-if", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-braces-around-statements", "readability-function-cognitive-complexity", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-function-size", "readability-identifier-length", "readability-magic-numbers", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				"readability-misleading-indentation", "readability-redundant-preprocessor", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-reference-to-constructed-temporary", "readability-suspicious-call-argument", //$NON-NLS-1$ //$NON-NLS-2$
				"readability-use-anyofallof"); //$NON-NLS-1$

		// zircon-*
		addChecks("zircon-temporary-objects"); //$NON-NLS-1$
	}

	// Collecting all checks
	static {
		ALL_CHECKS.addAll(CHECKS_WITHOUT_FIXES);
		ALL_CHECKS.addAll(AUTO_FIXABLE_CHECKS);
		ALL_CHECKS.addAll(RENAME_FIXABLE_CHECKS);
	}

	private static void addAutoFixableChecks(String... checks) {
		Collections.addAll(AUTO_FIXABLE_CHECKS, checks);
	}

	private static void addRenameFixableChecks(String... checks) {
		Collections.addAll(RENAME_FIXABLE_CHECKS, checks);
	}

	private static void addChecks(String... checks) {
		Collections.addAll(CHECKS_WITHOUT_FIXES, checks);
	}

	/**
	 * Checks whether checkId supports auto quick fix.
	 * 
	 * @param checkId check id.
	 * @return True if check supports auto quick fix, else false.
	 */
	public static boolean supportsAutoFix(String checkId) {
		return AUTO_FIXABLE_CHECKS.contains(checkId);
	}

	/**
	 * Checks whether checkId supports rename quick fix.
	 * 
	 * @param checkId check id.
	 * @return True if check supports rename quick fix, else false.
	 */
	public static boolean supportsRenameFix(String checkId) {
		return RENAME_FIXABLE_CHECKS.contains(checkId);
	}

	/**
	 * Checks whether checkId supports available quick fixes.
	 * 
	 * @param checkId check id.
	 * @return True if check supports any quick fix, else false.
	 */
	public static boolean supportsFix(String checkId) {
		return supportsAutoFix(checkId) || supportsRenameFix(checkId);
	}

	/**
	 * Gets all clang-tidy checks.
	 * 
	 * @return set of all clang-tidy checks.
	 */
	public static Set<String> getAll() {
		return ALL_CHECKS;
	}
}
