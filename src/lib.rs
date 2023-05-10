#![cfg_attr(not(feature = "std"), no_std)]

#[cfg(feature = "export-src")]
pub fn source_dir() -> std::path::PathBuf {
    std::path::Path::new(env!("CARGO_MANIFEST_DIR")).to_path_buf()
}
