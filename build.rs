use std::collections::HashMap;
use std::env;

use semver::Version;

use clang::{token::TokenKind::Punctuation, Clang, Index};

fn main() {
    println!("cargo:source_dir={}", env!("CARGO_MANIFEST_DIR"));

    let ver = env!("CARGO_PKG_VERSION");
    let ver = Version::parse(ver).unwrap();
    dbg!(&ver);

    let header_ver = get_core_version_from_header();
    dbg!(&header_ver);

    assert_eq!(ver, header_ver);
}

fn get_core_version_from_header() -> Version {
    let macros = get_definitions("c2a_core_main.h");

    let major: u64 = macros
        .get("C2A_CORE_VER_MAJOR")
        .expect("failed to get major ver")
        .as_ref()
        .unwrap()
        .parse()
        .expect("failed to parse as u64");
    let minor: u64 = macros
        .get("C2A_CORE_VER_MINOR")
        .expect("failed to get minor ver")
        .as_ref()
        .unwrap()
        .parse()
        .expect("failed to parse as u64");
    let patch: u64 = macros
        .get("C2A_CORE_VER_PATCH")
        .expect("failed to get patch ver")
        .as_ref()
        .unwrap()
        .parse()
        .expect("failed to parse as u64");
    let pre = macros
        .get("C2A_CORE_VER_PRE")
        .expect("failed to get pre ver")
        .as_ref()
        .unwrap();
    let pre = semver::Prerelease::new(pre).expect("failed to parse as pre release");

    Version {
        major,
        minor,
        patch,
        pre,
        build: semver::BuildMetadata::EMPTY,
    }
}

fn get_definitions(src_file: &str) -> HashMap<String, Option<String>> {
    let mut macros = HashMap::new();

    let clang = Clang::new().expect("failed to acquire clang instance");
    let index = Index::new(&clang, false, false);

    let tu = index
        .parser(src_file)
        .detailed_preprocessing_record(true)
        .parse()
        .expect("failed to parse c2a-core main header");
    let entity = tu.get_entity();

    let childlen = entity.get_children().into_iter();

    for cursor in childlen {
        match cursor.get_kind() {
            clang::EntityKind::MacroDefinition => {
                let location = cursor.get_location().unwrap().get_file_location();
                if let Some(file) = location.file {
                    let file = file.get_path();
                    let _f = file.to_str().unwrap();
                } else {
                    continue;
                }

                let name = cursor.get_display_name().unwrap();
                let mut token = cursor.get_range().unwrap().tokenize();
                token.remove(0); // remove macro Identifier token
                if token.is_empty() {
                    macros.insert(name, None);
                    continue; // remove define only
                }

                let first = token.first().unwrap();
                let last = token.last().unwrap();
                if first.get_kind() == Punctuation && last.get_kind() == Punctuation && first.get_spelling() == "(" && last.get_spelling() == ")" {
                    token.remove(0);
                    token.remove(token.len() - 1);
                }

                if token.len() == 1 {
                    let value = token[0].get_spelling();

                    let value = if value.starts_with('\"') && value.ends_with('\"') {
                        let value = value.strip_prefix('\"').unwrap();
                        value.strip_suffix('\"').unwrap().to_string()
                    } else {
                        value
                    };
                    macros.insert(name, Some(value));
                } else {
                    // 単純な値ではなかった(ex: 関数マクロ)
                    dbg!(token);
                }
            }
            _ => {}
        }
    }

    macros
}
