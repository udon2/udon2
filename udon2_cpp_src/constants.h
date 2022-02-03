/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_CONSTANTS_H_
#define UDON2_CPP_SRC_CONSTANTS_H_

#include <map>
#include <string>
#include <vector>

namespace constants {
struct UniversalToken {
  std::string code;
  std::string name;

  bool operator<(const UniversalToken& o) const { return code < o.code; }
  bool operator==(const UniversalToken& o) const { return code == o.code; }

  std::string toString() { return code + "/" + name; }
};

const struct UniversalToken ADJ_UPOS = {"ADJ", "adjective"};
const struct UniversalToken ADP_UPOS = {"ADP", "adposition"};
const struct UniversalToken ADV_UPOS = {"ADV", "adverb"};
const struct UniversalToken AUX_UPOS = {"AUX", "auxiliary"};
const struct UniversalToken CCONJ_UPOS = {"CCONJ", "coordinating conjunction"};
const struct UniversalToken DET_UPOS = {"DET", "determiner"};
const struct UniversalToken INTJ_UPOS = {"INTJ", "interjection"};
const struct UniversalToken NOUN_UPOS = {"NOUN", "noun"};
const struct UniversalToken NUM_UPOS = {"NUM", "numeral"};
const struct UniversalToken PART_UPOS = {"PART", "particle"};
const struct UniversalToken PRON_UPOS = {"PRON", "pronoun"};
const struct UniversalToken PROPN_UPOS = {"PROPN", "proper noun"};
const struct UniversalToken PUNCT_UPOS = {"PUNCT", "punctuation"};
const struct UniversalToken SCONJ_UPOS = {"SCONJ", "subordinating conjunction"};
const struct UniversalToken SYM_UPOS = {"SYM", "symbol"};
const struct UniversalToken VERB_UPOS = {"VERB", "verb"};
const struct UniversalToken X_UPOS = {"X", "other"};

const int NUM_UPOS_TAGS = 17;

typedef struct UposArray {
  const UniversalToken tags[NUM_UPOS_TAGS] = {
      ADJ_UPOS,   ADV_UPOS,   ADP_UPOS, AUX_UPOS,  CCONJ_UPOS, DET_UPOS,
      INTJ_UPOS,  NOUN_UPOS,  NUM_UPOS, PART_UPOS, PRON_UPOS,  PROPN_UPOS,
      PUNCT_UPOS, SCONJ_UPOS, SYM_UPOS, VERB_UPOS, X_UPOS};

  const UniversalToken& operator[](int i) const { return tags[i]; }
  int size() { return NUM_UPOS_TAGS; }

  const UniversalToken* begin() { return std::begin(tags); }
  const UniversalToken* end() { return std::end(tags); }
} UposArray;

const UposArray UPOS_TAGS;

const struct UniversalToken ACL_DEPREL = {
    "acl", "clausal modifier of noun (adnominal clause)"};
const struct UniversalToken ACL__RELCL_DEPREL = {"acl:relcl",
                                                 "relative clause modifier"};
const struct UniversalToken ADVCL_DEPREL = {"advcl",
                                            "adverbial clause modifier"};
const struct UniversalToken ADVMOD_DEPREL = {"advmod", "adverbial modifier"};
const struct UniversalToken ADVMOD__EMPH_DEPREL = {
    "advmod:emph", "emphasizing word, intensifier"};
const struct UniversalToken ADVMOD__LMOD_DEPREL = {
    "advmod:lmod", "locative adverbial modifier"};
const struct UniversalToken AMOD_DEPREL = {"amod", "adjectival modifier"};
const struct UniversalToken APPOS_DEPREL = {"appos", "appositional modifier"};
const struct UniversalToken AUX_DEPREL = {"aux", "auxiliary"};
const struct UniversalToken AUX__PASS_DEPREL = {"aux:pass",
                                                "passive auxiliary"};
const struct UniversalToken CASE_DEPREL = {"case", "case marking"};
const struct UniversalToken CC_DEPREL = {"cc", "coordinating conjunction"};
const struct UniversalToken CC__PRECONJ_DEPREL = {"cc:preconj", "preconjunct"};
const struct UniversalToken CCOMP_DEPREL = {"ccomp", "clausal complement"};
const struct UniversalToken CLF_DEPREL = {"clf", "classifier"};
const struct UniversalToken COMPOUND_DEPREL = {"compound", "compound"};
const struct UniversalToken COMPOUND__LVC_DEPREL = {"compound:lvc",
                                                    "light verb construction"};
const struct UniversalToken COMPOUND__PRT_DEPREL = {"compound:prt",
                                                    "phrasal verb particle"};
const struct UniversalToken COMPOUND__REDUP_DEPREL = {"compound:redup",
                                                      "reduplicated compounds"};
const struct UniversalToken COMPOUNT__SVC_DEPREL = {"compound:svc",
                                                    "serial verb compounds"};
const struct UniversalToken CONJ_DEPREL = {"conj", "conjunct"};
const struct UniversalToken COP_DEPREL = {"cop", "copula"};
const struct UniversalToken CSUBJ_DEPREL = {"csubj", "clausal subject"};
const struct UniversalToken CSUBJ__PASS_DEPREL = {"csubj:pass",
                                                  "clausal passive subject"};
const struct UniversalToken DEP_DEPREL = {"dep", "unspecified dependency"};
const struct UniversalToken DET_DEPREL = {"det", "determiner"};
const struct UniversalToken DET__NUMGOV_DEPREL = {
    "det:numgov", "pronominal quantifier governing the case of the noun"};
const struct UniversalToken DET__NUMMOD_DEPREL = {
    "det:nummod", "pronominal quantifier agreeing in case with the noun"};
const struct UniversalToken DET__POSS_DEPREL = {"det:poss",
                                                "possessive determiner"};
const struct UniversalToken DISCOURSE_DEPREL = {"discourse",
                                                "discourse element"};
const struct UniversalToken DISLOCATED_DEPREL = {"dislocated",
                                                 "dislocated elements"};
const struct UniversalToken EXPL_DEPREL = {"expl", "expletive"};
const struct UniversalToken EXPL__IMPERS_DEPREL = {"expl:impers",
                                                   "impersonal expletive"};
const struct UniversalToken EXPL__PASS_DEPREL = {
    "expl:pass", "reflexive pronoun used in reflexive passive"};
const struct UniversalToken EXPL__PV_DEPREL = {
    "expl:pv", "reflexive clitic with an inherently reflexive verb"};
const struct UniversalToken FIXED_DEPREL = {"fixed",
                                            "fixed multiword expression"};
const struct UniversalToken FLAT_DEPREL = {"flat", "flat multiword expression"};
const struct UniversalToken FLAT__FOREIGN_DEPREL = {"flat:foreign",
                                                    "foreign words"};
const struct UniversalToken FLAT__NAME_DEPREL = {"flat:name", "names"};
const struct UniversalToken GOESWITH_DEPREL = {"goeswith", "goes with"};
const struct UniversalToken IOBJ_DEPREL = {"iobj", "indirect object"};
const struct UniversalToken LIST_DEPREL = {"list", "list"};
const struct UniversalToken MARK_DEPREL = {"mark", "marker"};
const struct UniversalToken NMOD_DEPREL = {"nmod", "nominal modifier"};
const struct UniversalToken NMOD__POSS_DEPREL = {"nmod:poss",
                                                 "possessive nominal modifier"};
const struct UniversalToken NMOD__TMOD_DEPREL = {"nmod:tmod",
                                                 "temporal modifier"};
const struct UniversalToken NSUBJ_DEPREL = {"nsubj", "nominal subject"};
const struct UniversalToken NSUBJ__PASS_DEPREL = {"nsubj:pass",
                                                  "passive nominal subject"};
const struct UniversalToken NUMMOD_DEPREL = {"nummod", "numeric modifier"};
const struct UniversalToken NUMMOD__GOV_DEPREL = {
    "nummod:gov", "numeric modifier governing the case of the noun"};
const struct UniversalToken OBJ_DEPREL = {"obj", "object"};
const struct UniversalToken OBL_DEPREL = {"obl", "oblique nominal"};
const struct UniversalToken OBL__AGENT_DEPREL = {"obl:agent", "agent modifier"};
const struct UniversalToken OBL__ARG_DEPREL = {"obl:arg", "oblique argument"};
const struct UniversalToken OBL__LMOD_DEPREL = {"obl:lmod",
                                                "locative modifier"};
const struct UniversalToken OBL__TMOD_DEPREL = {"obl:tmod",
                                                "temporal modifier"};
const struct UniversalToken ORPHAN_DEPREL = {"orphan", "orphan"};
const struct UniversalToken PARATAXIS_DEPREL = {"parataxis", "parataxis"};
const struct UniversalToken PUNCT_DEPREL = {"punct", "punctuation"};
const struct UniversalToken REPARANDUM_DEPREL = {"reparandum",
                                                 "overridden disfluency"};
const struct UniversalToken ROOT_DEPREL = {"root", "root"};
const struct UniversalToken VOCATIVE_DEPREL = {"vocative", "vocative"};
const struct UniversalToken XCOMP_DEPREL = {"xcomp", "open clausal complement"};

const int NUM_DEPRELS = 63;

typedef struct DeprelArray {
  const UniversalToken deprels[NUM_DEPRELS] = {
      ACL_DEPREL,
      ACL__RELCL_DEPREL,
      ADVCL_DEPREL,
      ADVMOD_DEPREL,
      ADVMOD__EMPH_DEPREL,
      ADVMOD__LMOD_DEPREL,
      AMOD_DEPREL,
      APPOS_DEPREL,
      AUX_DEPREL,
      AUX__PASS_DEPREL,
      CASE_DEPREL,
      CC_DEPREL,
      CC__PRECONJ_DEPREL,
      CCOMP_DEPREL,
      CLF_DEPREL,
      COMPOUND_DEPREL,
      COMPOUND__LVC_DEPREL,
      COMPOUND__PRT_DEPREL,
      COMPOUND__REDUP_DEPREL,
      COMPOUNT__SVC_DEPREL,
      CONJ_DEPREL,
      COP_DEPREL,
      CSUBJ_DEPREL,
      CSUBJ__PASS_DEPREL,
      DEP_DEPREL,
      DET_DEPREL,
      DET__NUMGOV_DEPREL,
      DET__NUMMOD_DEPREL,
      DET__POSS_DEPREL,
      DISCOURSE_DEPREL,
      DISLOCATED_DEPREL,
      EXPL_DEPREL,
      EXPL__IMPERS_DEPREL,
      EXPL__PASS_DEPREL,
      EXPL__PV_DEPREL,
      FIXED_DEPREL,
      FLAT_DEPREL,
      FLAT__FOREIGN_DEPREL,
      FLAT__NAME_DEPREL,
      GOESWITH_DEPREL,
      IOBJ_DEPREL,
      LIST_DEPREL,
      MARK_DEPREL,
      NMOD_DEPREL,
      NMOD__POSS_DEPREL,
      NMOD__TMOD_DEPREL,
      NSUBJ_DEPREL,
      NSUBJ__PASS_DEPREL,
      NUMMOD_DEPREL,
      NUMMOD__GOV_DEPREL,
      OBJ_DEPREL,
      OBL_DEPREL,
      OBL__AGENT_DEPREL,
      OBL__ARG_DEPREL,
      OBL__LMOD_DEPREL,
      OBL__TMOD_DEPREL,
      ORPHAN_DEPREL,
      PARATAXIS_DEPREL,
      PUNCT_DEPREL,
      REPARANDUM_DEPREL,
      ROOT_DEPREL,
      VOCATIVE_DEPREL,
      XCOMP_DEPREL,
  };

  const UniversalToken& operator[](int i) const { return deprels[i]; }
  int size() { return NUM_DEPRELS; }

  const UniversalToken* begin() { return std::begin(deprels); }
  const UniversalToken* end() { return std::end(deprels); }
} DeprelArray;

const DeprelArray DEPRELS;

typedef std::vector<UniversalToken> UniversalTokenList;
typedef std::map<std::string, UniversalTokenList> UniversalMap;

/**
 * Universal features as defined here:
 * https://universaldependencies.org/u/feat/all.html
 */

const struct UniversalToken ABBR_UFEAT = {"Abbr", "abbreviation"};
const struct UniversalToken ADPTYPE_UFEAT = {"AdpType", "adposition type"};
const struct UniversalToken ADVTYPE_UFEAT = {"AdvType", "adverb type"};
const struct UniversalToken ANIMACY_UFEAT = {"Animacy", "animacy"};
const struct UniversalToken ASPECT_UFEAT = {"Aspect", "aspect"};
const struct UniversalToken CASE_UFEAT = {"Case", "case"};
const struct UniversalToken CLUSIVITY_UFEAT = {"Clusivity", "clusivity"};
const struct UniversalToken CONJTYPE_UFEAT = {"ConjType", "conjunction type"};
const struct UniversalToken DEFINITE_UFEAT = {"Definite",
                                              "definiteness or state"};
const struct UniversalToken DEGREE_UFEAT = {"Degree", "degree of comparison"};
const struct UniversalToken DEIXIS_UFEAT = {
    "Deixis", "relative location encoded in demonstratives"};
const struct UniversalToken DEIXISREF_UFEAT = {
    "DeixisRef", "person to which deixis is relative"};
const struct UniversalToken ECHO_UFEAT = {
    "Echo", "is this an echo word or a reduplicative?"};
const struct UniversalToken EVIDENT_UFEAT = {"Evident", "evidentiality"};
const struct UniversalToken FOREIGN_UFEAT = {"Foreign",
                                             "is this a foreign word?"};
const struct UniversalToken GENDER_UFEAT = {"Gender", "gender"};
const struct UniversalToken GENDER__DAT_UFEAT = {
    "Gender[dat]", "gender agreement with the dative argument"};
const struct UniversalToken GENDER__ERG_UFEAT = {
    "Gender[erg]", "gender agreement with the ergative argument"};
const struct UniversalToken GENDER__OBJ_UFEAT = {
    "Gender[obj]", "gender agreement with object"};
const struct UniversalToken GENDER__PSOR_UFEAT = {"Gender[psor]",
                                                  "possessor’s gender"};
const struct UniversalToken GENDER__SUBJ_UFEAT = {
    "Gender[subj]", "gender agreement with subject"};
const struct UniversalToken HYPH_UFEAT = {"Hyph",
                                          "hyphenated compound or part of it"};
const struct UniversalToken MOOD_UFEAT = {"Mood", "mood"};
const struct UniversalToken NAMETYPE_UFEAT = {"NameType",
                                              "type of named entity"};
const struct UniversalToken NOUNCLASS_UFEAT = {"NounClass", "noun class"};
const struct UniversalToken NOUNTYPE_UFEAT = {"NounType", "noun type"};
const struct UniversalToken NUMFORM_UFEAT = {"NumForm", "numeral form"};
const struct UniversalToken NUMTYPE_UFEAT = {"NumType", "numeral type"};
const struct UniversalToken NUMVALUE_UFEAT = {"NumValue", "numeric value"};
const struct UniversalToken NUMBER_UFEAT = {"Number", "number"};
const struct UniversalToken NUMBER__ABS_UFEAT = {
    "Number[abs]", "number agreement with absolutive argument"};
const struct UniversalToken NUMBER__DAT_UFEAT = {
    "Number[dat]", "number agreement with dative argument"};
const struct UniversalToken NUMBER__ERG_UFEAT = {
    "Number[erg]", "number agreement with ergative argument"};
const struct UniversalToken NUMBER__OBJ_UFEAT = {
    "Number[obj]", "number agreement with object"};
const struct UniversalToken NUMBER__PSED_UFEAT = {"Number[psed]",
                                                  "possessed object’s number"};
const struct UniversalToken NUMBER__PSOR_UFEAT = {"Number[psor]",
                                                  "possessor’s number"};
const struct UniversalToken NUMBER__SUBJ_UFEAT = {
    "Number[subj]", "number agreement with subject"};
const struct UniversalToken PARTTYPE_UFEAT = {"PartType", "particle type"};
const struct UniversalToken PERSON_UFEAT = {"Person", "person"};
const struct UniversalToken PERSON__ABS_UFEAT = {
    "Person[abs]", "person agreement with the absolutive argument"};
const struct UniversalToken PERSON__DAT_UFEAT = {
    "Person[dat]", "person agreement with the dative argument"};
const struct UniversalToken PERSON__ERG_UFEAT = {
    "Person[erg]", "person agreement with the ergative argument"};
const struct UniversalToken PERSON__OBJ_UFEAT = {
    "Person[obj]", "person agreement with object"};
const struct UniversalToken PERSON__PSOR_UFEAT = {"Person[psor]",
                                                  "possessor’s person"};
const struct UniversalToken PERSON__SUBJ_UFEAT = {
    "Person[subj]", "person agreement with subject"};
const struct UniversalToken POLARITY_UFEAT = {"Polarity", "polarity"};
const struct UniversalToken POLITE_UFEAT = {"Polite", "politeness"};
const struct UniversalToken POLITE__ABS_UFEAT = {
    "Polite[abs]", "politeness agreement with absolutive argument"};
const struct UniversalToken POLITE__DAT_UFEAT = {
    "Polite[dat]", "politeness agreement with dative argument"};
const struct UniversalToken POLITE__ERG_UFEAT = {
    "Polite[erg]", "politeness agreement with ergative argument"};
const struct UniversalToken POSS_UFEAT = {"Poss", "possessive"};
const struct UniversalToken PREPCASE_UFEAT = {
    "PrepCase", "case form sensitive to prepositions"};
const struct UniversalToken PRONTYPE_UFEAT = {"PronType", "pronominal type"};
const struct UniversalToken PUNCTSIDE_UFEAT = {
    "PunctSide", "which side of paired punctuation is this?"};
const struct UniversalToken PUNCTTYPE_UFEAT = {"PunctType", "punctuation type"};
const struct UniversalToken REFLEX_UFEAT = {"Reflex", "reflexive"};
const struct UniversalToken STYLE_UFEAT = {
    "Style", "style or sublanguage to which this word form belongs"};
const struct UniversalToken SUBCAT_UFEAT = {"Subcat", "subcategorization"};
const struct UniversalToken TENSE_UFEAT = {"Tense", "tense"};
const struct UniversalToken TYPO_UFEAT = {"Typo", "is this a misspelled word?"};
const struct UniversalToken VERBFORM_UFEAT = {"VerbForm",
                                              "form of verb or deverbative"};
const struct UniversalToken VERBTYPE_UFEAT = {"VerbType", "verb type"};
const struct UniversalToken VOICE_UFEAT = {"Voice", "voice"};

const int NUM_UFEATS = 63;

typedef struct UfeatArray {
  const UniversalToken feats[NUM_UFEATS] = {
      ABBR_UFEAT,         ADPTYPE_UFEAT,      ADVTYPE_UFEAT,
      ANIMACY_UFEAT,      ASPECT_UFEAT,       CASE_UFEAT,
      CLUSIVITY_UFEAT,    CONJTYPE_UFEAT,     DEFINITE_UFEAT,
      DEGREE_UFEAT,       DEIXIS_UFEAT,       DEIXISREF_UFEAT,
      ECHO_UFEAT,         EVIDENT_UFEAT,      FOREIGN_UFEAT,
      GENDER_UFEAT,       GENDER__DAT_UFEAT,  GENDER__ERG_UFEAT,
      GENDER__OBJ_UFEAT,  GENDER__PSOR_UFEAT, GENDER__SUBJ_UFEAT,
      HYPH_UFEAT,         MOOD_UFEAT,         NAMETYPE_UFEAT,
      NOUNCLASS_UFEAT,    NOUNTYPE_UFEAT,     NUMFORM_UFEAT,
      NUMTYPE_UFEAT,      NUMVALUE_UFEAT,     NUMBER_UFEAT,
      NUMBER__ABS_UFEAT,  NUMBER__DAT_UFEAT,  NUMBER__ERG_UFEAT,
      NUMBER__OBJ_UFEAT,  NUMBER__PSED_UFEAT, NUMBER__PSOR_UFEAT,
      NUMBER__SUBJ_UFEAT, PARTTYPE_UFEAT,     PERSON_UFEAT,
      PERSON__ABS_UFEAT,  PERSON__DAT_UFEAT,  PERSON__ERG_UFEAT,
      PERSON__OBJ_UFEAT,  PERSON__PSOR_UFEAT, PERSON__SUBJ_UFEAT,
      POLARITY_UFEAT,     POLITE_UFEAT,       POLITE__ABS_UFEAT,
      POLITE__DAT_UFEAT,  POLITE__ERG_UFEAT,  POSS_UFEAT,
      PREPCASE_UFEAT,     PRONTYPE_UFEAT,     PUNCTSIDE_UFEAT,
      PUNCTTYPE_UFEAT,    REFLEX_UFEAT,       STYLE_UFEAT,
      SUBCAT_UFEAT,       TENSE_UFEAT,        TYPO_UFEAT,
      VERBFORM_UFEAT,     VERBTYPE_UFEAT,     VOICE_UFEAT,
  };

  const UniversalToken& operator[](int i) const { return feats[i]; }
  int size() { return NUM_UFEATS; }

  const UniversalToken* begin() { return std::begin(feats); }
  const UniversalToken* end() { return std::end(feats); }
} UfeatArray;

const UfeatArray UFEATS;

/**
 * Values for universal features as defined here:
 * https://universaldependencies.org/u/feat/all.html
 */
const struct UniversalToken ABBR__YES_UVAL = {"Yes", "it is abbreviation"};
const struct UniversalToken ADPTYPE__PREP_UVAL = {"Prep", "preposition"};
const struct UniversalToken ADPTYPE__UPOST_UVAL = {"Post", "postposition"};
const struct UniversalToken ADPTYPE__CIRC_UVAL = {"Circ", "circumposition"};
const struct UniversalToken ADPTYPE__VOC_UVAL = {"Voc",
                                                 "vocalized preposition"};
const struct UniversalToken ADVTYPE__MAN_UVAL = {"Man", "adverb of manner"};
const struct UniversalToken ADVTYPE__LOC_UVAL = {"Loc", "adverb of location"};
const struct UniversalToken ADVTYPE__TIM_UVAL = {"Tim", "adverb of time"};
const struct UniversalToken ADVTYPE__DEG_UVAL = {
    "Deg", "adverb of quantity or degree"};
const struct UniversalToken ADVTYPE__CAU_UVAL = {"Cau", "adverb of cause"};
const struct UniversalToken ADVTYPE__MOD_UVAL = {"Mod",
                                                 "adverb of modal nature"};
const struct UniversalToken ANIMACY__ANIM_UVAL = {"Anim", "animate"};
const struct UniversalToken ANIMACY__INAN_UVAL = {"Inan", "inanimate"};
const struct UniversalToken ANIMACY__HUM_UVAL = {"Hum", "human"};
const struct UniversalToken ANIMACY__NHUM_UVAL = {"Nhum", "non-human"};
const struct UniversalToken ASPECT__IMP_UVAL = {"Imp", "imperfect aspect"};
const struct UniversalToken ASPECT__PERF_UVAL = {"Perf", "perfect aspect"};
const struct UniversalToken ASPECT__PROSP_UVAL = {"Prosp",
                                                  "prospective aspect"};
const struct UniversalToken ASPECT__PROG_UVAL = {"Prog", "progressive aspect"};
const struct UniversalToken ASPECT__HAB_UVAL = {"Hab", "habitual aspect"};
const struct UniversalToken ASPECT__ITER_UVAL = {
    "Iter", "iterative / frequentative aspect"};
const struct UniversalToken CASE__NOM_UVAL = {"Nom", "nominative / direct"};
const struct UniversalToken CASE__ACC_UVAL = {"Acc", "accusative / oblique"};
const struct UniversalToken CASE__ABS_UVAL = {"Abs", "absolutive"};
const struct UniversalToken CASE__ERG_UVAL = {"Erg", "ergative"};
const struct UniversalToken CASE__DAT_UVAL = {"Dat", "dative"};
const struct UniversalToken CASE__GEN_UVAL = {"Gen", "genitive"};
const struct UniversalToken CASE__VOC_UVAL = {"Voc", "vocative"};
const struct UniversalToken CASE__LOC_UVAL = {"Loc", "locative"};
const struct UniversalToken CASE__INS_UVAL = {"Ins",
                                              "instrumental / instructive"};
const struct UniversalToken CASE__PAR_UVAL = {"Par", "partitive"};
const struct UniversalToken CASE__DIS_UVAL = {"Dis", "distributive"};
const struct UniversalToken CASE__ESS_UVAL = {"Ess", "essive / prolative"};
const struct UniversalToken CASE__TRA_UVAL = {"Tra", "translative / factive"};
const struct UniversalToken CASE__COM_UVAL = {"Com",
                                              "comitative / associative"};
const struct UniversalToken CASE__ABE_UVAL = {"Abe", "abessive"};
const struct UniversalToken CASE__INE_UVAL = {"Ine", "inessive"};
const struct UniversalToken CASE__ILL_UVAL = {"Ill", "illative"};
const struct UniversalToken CASE__ELA_UVAL = {"Ela", "elative"};
const struct UniversalToken CASE__ADD_UVAL = {"Add", "additive"};
const struct UniversalToken CASE__ADE_UVAL = {"Ade", "adessive"};
const struct UniversalToken CASE__ALL_UVAL = {"All", "allative"};
const struct UniversalToken CASE__ABL_UVAL = {"Abl", "ablative"};
const struct UniversalToken CASE__SUP_UVAL = {"Sup", "superessive"};
const struct UniversalToken CASE__SUB_UVAL = {"Sub", "sublative"};
const struct UniversalToken CASE__DEL_UVAL = {"Del", "delative"};
const struct UniversalToken CASE__LAT_UVAL = {"Lat",
                                              "lative / directional allative"};
const struct UniversalToken CASE__PER_UVAL = {"Per", "perlative"};
const struct UniversalToken CASE__TEM_UVAL = {"Tem", "temporal"};
const struct UniversalToken CASE__TER_UVAL = {
    "Ter", "terminative / terminal allative"};
const struct UniversalToken CASE__CAU_UVAL = {
    "Cau", "causative / motivative / purposive"};
const struct UniversalToken CASE__BEN_UVAL = {"Ben",
                                              "benefactive / destinative"};
const struct UniversalToken CASE__CNS_UVAL = {"Cns", "considerative"};
const struct UniversalToken CASE__CMP_UVAL = {"Cmp", "comparative"};
const struct UniversalToken CASE__EQU_UVAL = {"Equ", "equative"};
const struct UniversalToken CLUSIVITY__IN_UVAL = {"In", "inclusive"};
const struct UniversalToken CLUSIVITY__EX_UVAL = {"Ex", "exclusive"};
const struct UniversalToken CONJTYPE__COMP_UVAL = {"Comp",
                                                   "comparing conjunction"};
const struct UniversalToken CONJTYPE__OPER_UVAL = {"Oper",
                                                   "mathematical operator"};
const struct UniversalToken DEFINITE__IND_UVAL = {"Ind", "indefinite"};
const struct UniversalToken DEFINITE__SPEC_UVAL = {"Spec",
                                                   "specific indefinite"};
const struct UniversalToken DEFINITE__DEF_UVAL = {"Def", "definite"};
const struct UniversalToken DEFINITE__CONS_UVAL = {
    "Cons", "construct state / reduced definiteness"};
const struct UniversalToken DEFINITE__COM_UVAL = {"Com", "complex"};
const struct UniversalToken DEGREE__UPOS_UVAL = {"Pos",
                                                 "positive, first degree"};
const struct UniversalToken DEGREE__EQU_UVAL = {"Equ", "equative"};
const struct UniversalToken DEGREE__CMP_UVAL = {"Cmp",
                                                "comparative, second degree"};
const struct UniversalToken DEGREE__SUP_UVAL = {"Sup",
                                                "superlative, third degree"};
const struct UniversalToken DEGREE__ABS_UVAL = {"Abs", "absolute superlative"};
const struct UniversalToken DEIXIS__PROX_UVAL = {"Prox", "proximate"};
const struct UniversalToken DEIXIS__MED_UVAL = {"Med", "medial"};
const struct UniversalToken DEIXIS__REMT_UVAL = {"Remt", "remote, distal"};
const struct UniversalToken DEIXIS__NVIS_UVAL = {"Nvis", "not visible"};
const struct UniversalToken DEIXIS__ABV_UVAL = {"Abv",
                                                "above the reference point"};
const struct UniversalToken DEIXIS__EVEN_UVAL = {
    "Even", "at the same level as the reference point"};
const struct UniversalToken DEIXIS__BEL_UVAL = {"Bel",
                                                "below the reference point"};
const struct UniversalToken DEIXISREF__1_UVAL = {
    "1", "deixis relative to the first person participant (speaker)"};
const struct UniversalToken DEIXISREF__2_UVAL = {
    "2", "deixis relative to the second person participant (hearer)"};
const struct UniversalToken ECHO__RDP_UVAL = {"Rdp", "reduplicative"};
const struct UniversalToken ECHO__ECH_UVAL = {"Ech", "echo"};
const struct UniversalToken EVIDENT__FH_UVAL = {"Fh", "firsthand"};
const struct UniversalToken EVIDENT__NFH_UVAL = {"Nfh", "non-firsthand"};
const struct UniversalToken FOREIGN__YES_UVAL = {"Yes", "it is foreign"};
const struct UniversalToken GENDER__MASC_UVAL = {"Masc", "masculine gender"};
const struct UniversalToken GENDER__FEM_UVAL = {"Fem", "feminine gender"};
const struct UniversalToken GENDER__NEUT_UVAL = {"Neut", "neuter gender"};
const struct UniversalToken GENDER__COM_UVAL = {"Com", "common gender"};
const struct UniversalToken GENDER__DAT__MASC_UVAL = {
    "Masc", "masculine dative argument"};
const struct UniversalToken GENDER__DAT__FEM_UVAL = {
    "Fem", "feminine dative argument"};
const struct UniversalToken GENDER__ERG__MASC_UVAL = {
    "Masc", "masculine ergative argument"};
const struct UniversalToken GENDER__ERG__FEM_UVAL = {
    "Fem", "feminine dative argument"};
const struct UniversalToken GENDER__OBJ__MASC_UVAL = {"Masc",
                                                      "masculine object"};
const struct UniversalToken GENDER__OBJ__FEM_UVAL = {"Fem", "feminine object"};
const struct UniversalToken GENDER__PSOR__MASC_UVAL = {"Masc",
                                                       "masculine possessor"};
const struct UniversalToken GENDER__PSOR__FEM_UVAL = {"Fem",
                                                      "feminine possessor"};
const struct UniversalToken GENDER__PSOR__NEUT_UVAL = {"Neut",
                                                       "neuter possessor"};
const struct UniversalToken GENDER__SUBJ__MASC_UVAL = {"Masc",
                                                       "masculine subject"};
const struct UniversalToken GENDER__SUBJ__FEM_UVAL = {"Fem",
                                                      "feminine subject"};
const struct UniversalToken HYPH__YES_UVAL = {
    "Yes", "it is part of hyphenated compound"};
const struct UniversalToken MOOD__IND_UVAL = {"Ind", "indicative or realis"};
const struct UniversalToken MOOD__IMP_UVAL = {"Imp", "imperative"};
const struct UniversalToken MOOD__CND_UVAL = {"Cnd", "conditional"};
const struct UniversalToken MOOD__POT_UVAL = {"Pot", "potential"};
const struct UniversalToken MOOD__SUB_UVAL = {"Sub",
                                              "subjunctive / conjunctive"};
const struct UniversalToken MOOD__JUS_UVAL = {"Jus", "jussive / injunctive"};
const struct UniversalToken MOOD__PRP_UVAL = {"Prp", "purposive"};
const struct UniversalToken MOOD__QOT_UVAL = {"Qot", "quotative"};
const struct UniversalToken MOOD__OPT_UVAL = {"Opt", "optative"};
const struct UniversalToken MOOD__DES_UVAL = {"Des", "desiderative"};
const struct UniversalToken MOOD__NEC_UVAL = {"Nec", "necessitative"};
const struct UniversalToken MOOD__IRR_UVAL = {"Irr", "irrealis"};
const struct UniversalToken MOOD__ADM_UVAL = {"Adm", "admirative"};
const struct UniversalToken NAMETYPE__GEO_UVAL = {"Geo", "geographical name"};
const struct UniversalToken NAMETYPE__PRS_UVAL = {"Prs", "name of person"};
const struct UniversalToken NAMETYPE__GIV_UVAL = {"Giv",
                                                  "given name of person"};
const struct UniversalToken NAMETYPE__SUR_UVAL = {
    "Sur", "surname / family name of person"};
const struct UniversalToken NAMETYPE__NAT_UVAL = {"Nat", "nationality"};
const struct UniversalToken NAMETYPE__COM_UVAL = {"Com",
                                                  "company, organization"};
const struct UniversalToken NAMETYPE__PRO_UVAL = {"Pro", "product"};
const struct UniversalToken NAMETYPE__OTH_UVAL = {"Oth", "other"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU1_UVAL = {
    "Bantu1", "singular, persons"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU2_UVAL = {
    "Bantu2", "plural, persons"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU3_UVAL = {
    "Bantu3", "singular, plants, thin objects"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU4_UVAL = {
    "Bantu4", "plural, plants, thin objects"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU5_UVAL = {
    "Bantu5", "singular, fruits, round objects, paired things"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU6_UVAL = {
    "Bantu6", "plural, fruits, round objects, paired things"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU7_UVAL = {
    "Bantu7", "singular, things, diminutives"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU8_UVAL = {
    "Bantu8", "plural, things, diminutives"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU9_UVAL = {
    "Bantu9", "singular, animals, things"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU10_UVAL = {
    "Bantu10", "plural, animals, things"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU11_UVAL = {
    "Bantu11", "long thin objects, natural phenomena, abstracts"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU12_UVAL = {
    "Bantu12", "singular, small things, diminutives"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU13_UVAL = {
    "Bantu13", "plural or mass, small amount of mass"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU14_UVAL = {
    "Bantu14", "plural, diminutives"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU15_UVAL = {
    "Bantu15", "verbal nouns, infinitives"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU16_UVAL = {
    "Bantu16", "definite location, close to something"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU17_UVAL = {
    "Bantu17", "indefinite location, direction, movement"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU18_UVAL = {
    "Bantu18", "definite location, inside something"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU19_UVAL = {
    "Bantu19", "little bit of, pejorative plural"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU20_UVAL = {
    "Bantu20", "singular, augmentatives"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU21_UVAL = {
    "Bantu21", "singular, augmentatives, derogatives"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU22_UVAL = {
    "Bantu22", "plural, augmentatives"};
const struct UniversalToken NOUN_CLASSES_IN_SWAHILI__BANTU23_UVAL = {
    "Bantu23", "location with place names"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL1_UVAL = {
    "Wol1", "Wolof noun class 1/k (singular human)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL2_UVAL = {
    "Wol2", "Wolof noun class 2/ñ (plural human)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL3_UVAL = {
    "Wol3", "Wolof noun class 3/g (singular)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL4_UVAL = {
    "Wol4", "Wolof noun class 4/j (singular)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL5_UVAL = {
    "Wol5", "Wolof noun class 5/b (singular)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL6_UVAL = {
    "Wol6", "Wolof noun class 6/m (singular)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL7_UVAL = {
    "Wol7", "Wolof noun class 7/l (singular)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL8_UVAL = {
    "Wol8", "Wolof noun class 8/y (plural non-human)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL9_UVAL = {
    "Wol9", "Wolof noun class 9/s (singular)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL10_UVAL = {
    "Wol10", "Wolof noun class 10/w (singular)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL11_UVAL = {
    "Wol11", "Wolof noun class 11/f (location)"};
const struct UniversalToken NOUN_CLASSES_IN_WOLOF__WOL12_UVAL = {
    "Wol12", "Wolof noun class 12/n (manner)"};
const struct UniversalToken NOUNTYPE__CLF_UVAL = {"Clf", "classifier"};
const struct UniversalToken NUMFORM__WORD_UVAL = {"Word",
                                                  "number expressed as word"};
const struct UniversalToken NUMFORM__DIGIT_UVAL = {
    "Digit", "number expressed using digits"};
const struct UniversalToken NUMFORM__ROMAN_UVAL = {"Roman", "roman numeral"};
const struct UniversalToken NUMTYPE__CARD_UVAL = {
    "Card",
    "cardinal number or corresponding interrogative / relative / indefinite / "
    "demonstrative word"};
const struct UniversalToken NUMTYPE__ORD_UVAL = {
    "Ord",
    "ordinal number or corresponding interrogative / relative / indefinite / "
    "demonstrative word"};
const struct UniversalToken NUMTYPE__MULT_UVAL = {
    "Mult",
    "multiplicative numeral or corresponding interrogative / relative / "
    "indefinite / demonstrative word"};
const struct UniversalToken NUMTYPE__FRAC_UVAL = {"Frac", "fraction"};
const struct UniversalToken NUMTYPE__SETS_UVAL = {
    "Sets", "number of sets of things; collective numeral"};
const struct UniversalToken NUMTYPE__DIST_UVAL = {"Dist",
                                                  "distributive numeral"};
const struct UniversalToken NUMTYPE__RANGE_UVAL = {"Range", "range of values"};
const struct UniversalToken NUMVALUE__1_UVAL = {"1", "numeric value 1"};
const struct UniversalToken NUMVALUE__2_UVAL = {"2", "numeric value 2"};
const struct UniversalToken NUMVALUE__3_UVAL = {"3", "numeric value 3 or 4"};
const struct UniversalToken NUMBER__SING_UVAL = {"Sing", "singular number"};
const struct UniversalToken NUMBER__PLUR_UVAL = {"Plur", "plural number"};
const struct UniversalToken NUMBER__DUAL_UVAL = {"Dual", "dual number"};
const struct UniversalToken NUMBER__TRI_UVAL = {"Tri", "trial number"};
const struct UniversalToken NUMBER__PAUC_UVAL = {"Pauc", "paucal number"};
const struct UniversalToken NUMBER__GRPA_UVAL = {"Grpa",
                                                 "greater paucal number"};
const struct UniversalToken NUMBER__GRPL_UVAL = {"Grpl",
                                                 "greater plural number"};
const struct UniversalToken NUMBER__INV_UVAL = {"Inv", "inverse number"};
const struct UniversalToken NUMBER__COUNT_UVAL = {"Count", "count plural"};
const struct UniversalToken NUMBER__PTAN_UVAL = {"Ptan", "plurale tantum"};
const struct UniversalToken NUMBER__COLL_UVAL = {
    "Coll", "collective / mass / singulare tantum"};
const struct UniversalToken NUMBER__ABS__SING_UVAL = {
    "Sing", "singular absolutive argument"};
const struct UniversalToken NUMBER__ABS__PLUR_UVAL = {
    "Plur", "plural absolutive argument"};
const struct UniversalToken NUMBER__DAT__SING_UVAL = {
    "Sing", "singular dative argument"};
const struct UniversalToken NUMBER__DAT__PLUR_UVAL = {"Plur",
                                                      "plural dative argument"};
const struct UniversalToken NUMBER__ERG__SING_UVAL = {
    "Sing", "singular ergative argument"};
const struct UniversalToken NUMBER__ERG__PLUR_UVAL = {
    "Plur", "plural ergative argument"};
const struct UniversalToken NUMBER__OBJ__SING_UVAL = {"Sing",
                                                      "singular object"};
const struct UniversalToken NUMBER__OBJ__PLUR_UVAL = {"Plur", "plural object"};
const struct UniversalToken NUMBER__PSED__SING_UVAL = {"Sing",
                                                       "singular possession"};
const struct UniversalToken NUMBER__PSED__PLUR_UVAL = {"Plur",
                                                       "plural possession"};
const struct UniversalToken NUMBER__PSOR__SING_UVAL = {"Sing",
                                                       "singular possessor"};
const struct UniversalToken NUMBER__PSOR__DUAL_UVAL = {"Dual",
                                                       "dual possessor"};
const struct UniversalToken NUMBER__PSOR__PLUR_UVAL = {"Plur",
                                                       "plural possessor"};
const struct UniversalToken NUMBER__SUBJ__SING_UVAL = {"Sing",
                                                       "singular subject"};
const struct UniversalToken NUMBER__SUBJ__PLUR_UVAL = {"Plur",
                                                       "plural subject"};
const struct UniversalToken PARTTYPE__MOD_UVAL = {"Mod", "modal particle"};
const struct UniversalToken PARTTYPE__EMP_UVAL = {"Emp",
                                                  "particle of emphasis"};
const struct UniversalToken PARTTYPE__INF_UVAL = {"Inf", "infinitive marker"};
const struct UniversalToken PARTTYPE__INT_UVAL = {"Int", "question particle"};
const struct UniversalToken PARTTYPE__NEG_UVAL = {"Neg", "negation particle"};
const struct UniversalToken PARTTYPE__VBP_UVAL = {
    "Vbp", "separated verb prefix in German"};
const struct UniversalToken PERSON__0_UVAL = {"0", "zero person"};
const struct UniversalToken PERSON__1_UVAL = {"1", "first person"};
const struct UniversalToken PERSON__2_UVAL = {"2", "second person"};
const struct UniversalToken PERSON__3_UVAL = {"3", "third person"};
const struct UniversalToken PERSON__4_UVAL = {"4", "fourth person"};
const struct UniversalToken PERSON__ABS__1_UVAL = {
    "1", "first person absolutive argument"};
const struct UniversalToken PERSON__ABS__2_UVAL = {
    "2", "second person absolutive argument"};
const struct UniversalToken PERSON__ABS__3_UVAL = {
    "3", "third person absolutive argument"};
const struct UniversalToken PERSON__DAT__1_UVAL = {
    "1", "first person dative argument"};
const struct UniversalToken PERSON__DAT__2_UVAL = {
    "2", "second person dative argument"};
const struct UniversalToken PERSON__DAT__3_UVAL = {
    "3", "third person dative argument"};
const struct UniversalToken PERSON__ERG__1_UVAL = {
    "1", "first person ergative argument"};
const struct UniversalToken PERSON__ERG__2_UVAL = {
    "2", "second person ergative argument"};
const struct UniversalToken PERSON__ERG__3_UVAL = {
    "3", "third person ergative argument"};
const struct UniversalToken PERSON__OBJ__1_UVAL = {"1", "first person object"};
const struct UniversalToken PERSON__OBJ__2_UVAL = {"2", "second person object"};
const struct UniversalToken PERSON__OBJ__3_UVAL = {"3", "third person object"};
const struct UniversalToken PERSON__PSOR__1_UVAL = {"1",
                                                    "first person possessor"};
const struct UniversalToken PERSON__PSOR__2_UVAL = {"2",
                                                    "second person possessor"};
const struct UniversalToken PERSON__PSOR__3_UVAL = {"3",
                                                    "third person possessor"};
const struct UniversalToken PERSON__SUBJ__1_UVAL = {"1",
                                                    "first person subject"};
const struct UniversalToken PERSON__SUBJ__2_UVAL = {"2",
                                                    "second person subject"};
const struct UniversalToken PERSON__SUBJ__3_UVAL = {"3",
                                                    "third person subject"};
const struct UniversalToken POLARITY__UPOS_UVAL = {"Pos",
                                                   "positive, affirmative"};
const struct UniversalToken POLARITY__NEG_UVAL = {"Neg", "negative"};
const struct UniversalToken POLITE__INFM_UVAL = {"Infm", "informal register"};
const struct UniversalToken POLITE__FORM_UVAL = {"Form", "formal register"};
const struct UniversalToken POLITE__ELEV_UVAL = {"Elev", "referent elevating"};
const struct UniversalToken POLITE__HUMB_UVAL = {"Humb", "speaker humbling"};
const struct UniversalToken POLITE__ABS__INFM_UVAL = {
    "Infm", "informal absolutive argument"};
const struct UniversalToken POLITE__ABS__FORM_UVAL = {
    "Form", "polite, formal absolutive argument"};
const struct UniversalToken POLITE__DAT__INFM_UVAL = {
    "Infm", "informal dative argument"};
const struct UniversalToken POLITE__DAT__FORM_UVAL = {
    "Form", "polite, formal dative argument"};
const struct UniversalToken POLITE__ERG__INFM_UVAL = {
    "Infm", "informal ergative argument"};
const struct UniversalToken POLITE__ERG__FORM_UVAL = {
    "Form", "polite, formal ergative argument"};
const struct UniversalToken POSS__YES_UVAL = {"Yes", "it is possessive"};
const struct UniversalToken PREPCASE__NPR_UVAL = {"Npr",
                                                  "non-prepositional case"};
const struct UniversalToken PREPCASE__PRE_UVAL = {"Pre", "prepositional case"};
const struct UniversalToken PRONTYPE__PRS_UVAL = {
    "Prs", "personal or possessive personal pronoun or determiner"};
const struct UniversalToken PRONTYPE__RCP_UVAL = {"Rcp", "reciprocal pronoun"};
const struct UniversalToken PRONTYPE__ART_UVAL = {"Art", "article"};
const struct UniversalToken PRONTYPE__INT_UVAL = {
    "Int", "interrogative pronoun, determiner, numeral or adverb"};
const struct UniversalToken PRONTYPE__REL_UVAL = {
    "Rel", "relative pronoun, determiner, numeral or adverb"};
const struct UniversalToken PRONTYPE__EXC_UVAL = {"Exc",
                                                  "exclamative determiner"};
const struct UniversalToken PRONTYPE__DEM_UVAL = {
    "Dem", "demonstrative pronoun, determiner, numeral or adverb"};
const struct UniversalToken PRONTYPE__EMP_UVAL = {"Emp", "emphatic determiner"};
const struct UniversalToken PRONTYPE__TOT_UVAL = {
    "Tot", "total (collective) pronoun, determiner or adverb"};
const struct UniversalToken PRONTYPE__NEG_UVAL = {
    "Neg", "negative pronoun, determiner or adverb"};
const struct UniversalToken PRONTYPE__IND_UVAL = {
    "Ind", "indefinite pronoun, determiner, numeral or adverb"};
const struct UniversalToken PUNCTSIDE__INI_UVAL = {
    "Ini", "initial (left bracket in English texts)"};
const struct UniversalToken PUNCTSIDE__FIN_UVAL = {
    "Fin", "final (right bracket in English texts)"};
const struct UniversalToken PUNCTTYPE__PERI_UVAL = {
    "Peri", "period at the end of sentence"};
const struct UniversalToken PUNCTTYPE__QEST_UVAL = {"Qest", "question mark"};
const struct UniversalToken PUNCTTYPE__EXCL_UVAL = {"Excl", "exclamation mark"};
const struct UniversalToken PUNCTTYPE__QUOT_UVAL = {
    "Quot", "quotation marks (various sorts in various languages)"};
const struct UniversalToken PUNCTTYPE__BRCK_UVAL = {"Brck", "bracket"};
const struct UniversalToken PUNCTTYPE__COMM_UVAL = {"Comm", "comma"};
const struct UniversalToken PUNCTTYPE__COLO_UVAL = {"Colo", "colon"};
const struct UniversalToken PUNCTTYPE__SEMI_UVAL = {"Semi", "semicolon"};
const struct UniversalToken PUNCTTYPE__DASH_UVAL = {"Dash", "dash, hyphen"};
const struct UniversalToken REFLEX__YES_UVAL = {"Yes", "it is reflexive"};
const struct UniversalToken STYLE__ARCH_UVAL = {"Arch", "archaic, obsolete"};
const struct UniversalToken STYLE__RARE_UVAL = {"Rare", "rare"};
const struct UniversalToken STYLE__FORM_UVAL = {"Form", "formal, literary"};
const struct UniversalToken STYLE__COLL_UVAL = {"Coll", "colloquial"};
const struct UniversalToken STYLE__VRNC_UVAL = {"Vrnc", "vernacular"};
const struct UniversalToken STYLE__SLNG_UVAL = {"Slng", "slang"};
const struct UniversalToken STYLE__EXPR_UVAL = {"Expr",
                                                "expressive, emotional"};
const struct UniversalToken STYLE__VULG_UVAL = {"Vulg", "vulgar"};
const struct UniversalToken SUBCAT__INTR_UVAL = {"Intr", "intransitive verb"};
const struct UniversalToken SUBCAT__TRAN_UVAL = {"Tran", "transitive verb"};
const struct UniversalToken TENSE__PAST_UVAL = {
    "Past", "past tense / preterite / aorist"};
const struct UniversalToken TENSE__PRES_UVAL = {
    "Pres", "present / non-past tense / aorist"};
const struct UniversalToken TENSE__FUT_UVAL = {"Fut", "future tense"};
const struct UniversalToken TENSE__IMP_UVAL = {"Imp", "imperfect"};
const struct UniversalToken TENSE__PQP_UVAL = {"Pqp", "pluperfect"};
const struct UniversalToken TYPO__YES_UVAL = {"Yes", "it is typo"};
const struct UniversalToken VERBFORM__FIN_UVAL = {"Fin", "finite verb"};
const struct UniversalToken VERBFORM__INF_UVAL = {"Inf", "infinitive"};
const struct UniversalToken VERBFORM__SUP_UVAL = {"Sup", "supine"};
const struct UniversalToken VERBFORM__PART_UVAL = {
    "Part", "participle, verbal adjective"};
const struct UniversalToken VERBFORM__CONV_UVAL = {
    "Conv", "converb, transgressive, adverbial participle, verbal adverb"};
const struct UniversalToken VERBFORM__GDV_UVAL = {"Gdv", "gerundive"};
const struct UniversalToken VERBFORM__GER_UVAL = {"Ger", "gerund"};
const struct UniversalToken VERBFORM__VNOUN_UVAL = {"Vnoun",
                                                    "verbal noun, masdar"};
const struct UniversalToken VERBTYPE__AUX_UVAL = {"Aux", "auxiliary verb"};
const struct UniversalToken VERBTYPE__COP_UVAL = {"Cop", "copula verb"};
const struct UniversalToken VERBTYPE__MOD_UVAL = {"Mod", "modal verb"};
const struct UniversalToken VERBTYPE__LIGHT_UVAL = {"Light",
                                                    "light (support) verb"};
const struct UniversalToken VOICE__ACT_UVAL = {"Act",
                                               "active or actor-focus voice"};
const struct UniversalToken VOICE__MID_UVAL = {"Mid", "middle voice"};
const struct UniversalToken VOICE__RCP_UVAL = {"Rcp", "reciprocal voice"};
const struct UniversalToken VOICE__PASS_UVAL = {
    "Pass", "passive or patient-focus voice"};
const struct UniversalToken VOICE__ANTIP_UVAL = {"Antip", "antipassive voice"};
const struct UniversalToken VOICE__LFOC_UVAL = {"Lfoc", "location-focus voice"};
const struct UniversalToken VOICE__BFOC_UVAL = {"Bfoc",
                                                "beneficiary-focus voice"};
const struct UniversalToken VOICE__DIR_UVAL = {"Dir", "direct voice"};
const struct UniversalToken VOICE__INV_UVAL = {"Inv", "inverse voice"};
const struct UniversalToken VOICE__CAU_UVAL = {"Cau", "causative voice"};

/**
 * Key-value pairs for Universal Features, as defined here
 * https://universaldependencies.org/u/feat/all.html
 */
const UniversalMap FEATS = {
    {ABBR_UFEAT.code, {ABBR__YES_UVAL}},
    {ADPTYPE_UFEAT.code,
     {ADPTYPE__PREP_UVAL, ADPTYPE__UPOST_UVAL, ADPTYPE__CIRC_UVAL,
      ADPTYPE__VOC_UVAL}},
    {ADVTYPE_UFEAT.code,
     {ADVTYPE__MAN_UVAL, ADVTYPE__LOC_UVAL, ADVTYPE__TIM_UVAL,
      ADVTYPE__DEG_UVAL, ADVTYPE__CAU_UVAL, ADVTYPE__MOD_UVAL}},
    {ANIMACY_UFEAT.code,
     {ANIMACY__ANIM_UVAL, ANIMACY__INAN_UVAL, ANIMACY__HUM_UVAL,
      ANIMACY__NHUM_UVAL}},
    {ASPECT_UFEAT.code,
     {ASPECT__IMP_UVAL, ASPECT__PERF_UVAL, ASPECT__PROSP_UVAL,
      ASPECT__PROG_UVAL, ASPECT__HAB_UVAL, ASPECT__ITER_UVAL}},
    {CASE_UFEAT.code,
     {CASE__NOM_UVAL, CASE__ACC_UVAL, CASE__ABS_UVAL, CASE__ERG_UVAL,
      CASE__DAT_UVAL, CASE__GEN_UVAL, CASE__VOC_UVAL, CASE__LOC_UVAL,
      CASE__INS_UVAL, CASE__PAR_UVAL, CASE__DIS_UVAL, CASE__ESS_UVAL,
      CASE__TRA_UVAL, CASE__COM_UVAL, CASE__ABE_UVAL, CASE__INE_UVAL,
      CASE__ILL_UVAL, CASE__ELA_UVAL, CASE__ADD_UVAL, CASE__ADE_UVAL,
      CASE__ALL_UVAL, CASE__ABL_UVAL, CASE__SUP_UVAL, CASE__SUB_UVAL,
      CASE__DEL_UVAL, CASE__LAT_UVAL, CASE__PER_UVAL, CASE__TEM_UVAL,
      CASE__TER_UVAL, CASE__CAU_UVAL, CASE__BEN_UVAL, CASE__CNS_UVAL,
      CASE__CMP_UVAL, CASE__EQU_UVAL}},
    {CLUSIVITY_UFEAT.code, {CLUSIVITY__IN_UVAL, CLUSIVITY__EX_UVAL}},
    {CONJTYPE_UFEAT.code, {CONJTYPE__COMP_UVAL, CONJTYPE__OPER_UVAL}},
    {DEFINITE_UFEAT.code,
     {DEFINITE__IND_UVAL, DEFINITE__SPEC_UVAL, DEFINITE__DEF_UVAL,
      DEFINITE__CONS_UVAL, DEFINITE__COM_UVAL}},
    {DEGREE_UFEAT.code,
     {DEGREE__UPOS_UVAL, DEGREE__EQU_UVAL, DEGREE__CMP_UVAL, DEGREE__SUP_UVAL,
      DEGREE__ABS_UVAL}},
    {DEIXIS_UFEAT.code,
     {DEIXIS__PROX_UVAL, DEIXIS__MED_UVAL, DEIXIS__REMT_UVAL, DEIXIS__NVIS_UVAL,
      DEIXIS__ABV_UVAL, DEIXIS__EVEN_UVAL, DEIXIS__BEL_UVAL, DEIXISREF__1_UVAL,
      DEIXISREF__2_UVAL}},
    {DEIXISREF_UFEAT.code, {DEIXISREF__1_UVAL, DEIXISREF__2_UVAL}},
    {ECHO_UFEAT.code, {ECHO__RDP_UVAL, ECHO__ECH_UVAL}},
    {EVIDENT_UFEAT.code, {EVIDENT__FH_UVAL, EVIDENT__NFH_UVAL}},
    {FOREIGN_UFEAT.code, {FOREIGN__YES_UVAL}},
    {GENDER_UFEAT.code,
     {GENDER__MASC_UVAL, GENDER__FEM_UVAL, GENDER__NEUT_UVAL,
      GENDER__COM_UVAL}},
    {GENDER__DAT_UFEAT.code, {GENDER__DAT__MASC_UVAL, GENDER__DAT__FEM_UVAL}},
    {GENDER__ERG_UFEAT.code, {GENDER__ERG__MASC_UVAL, GENDER__ERG__FEM_UVAL}},
    {GENDER__OBJ_UFEAT.code, {GENDER__OBJ__MASC_UVAL, GENDER__OBJ__FEM_UVAL}},
    {GENDER__PSOR_UFEAT.code,
     {GENDER__PSOR__MASC_UVAL, GENDER__PSOR__FEM_UVAL,
      GENDER__PSOR__NEUT_UVAL}},
    {GENDER__SUBJ_UFEAT.code,
     {GENDER__SUBJ__MASC_UVAL, GENDER__SUBJ__FEM_UVAL}},
    {HYPH_UFEAT.code, {HYPH__YES_UVAL}},
    {MOOD_UFEAT.code,
     {MOOD__IND_UVAL, MOOD__IMP_UVAL, MOOD__CND_UVAL, MOOD__POT_UVAL,
      MOOD__SUB_UVAL, MOOD__JUS_UVAL, MOOD__PRP_UVAL, MOOD__QOT_UVAL,
      MOOD__OPT_UVAL, MOOD__DES_UVAL, MOOD__NEC_UVAL, MOOD__IRR_UVAL,
      MOOD__ADM_UVAL}},
    {NAMETYPE_UFEAT.code,
     {NAMETYPE__GEO_UVAL, NAMETYPE__PRS_UVAL, NAMETYPE__GIV_UVAL,
      NAMETYPE__SUR_UVAL, NAMETYPE__NAT_UVAL, NAMETYPE__COM_UVAL,
      NAMETYPE__PRO_UVAL, NAMETYPE__OTH_UVAL}},
    {NOUNCLASS_UFEAT.code, {NOUN_CLASSES_IN_SWAHILI__BANTU1_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU2_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU3_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU4_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU5_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU6_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU7_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU8_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU9_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU10_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU11_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU12_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU13_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU14_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU15_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU16_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU17_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU18_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU19_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU20_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU21_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU22_UVAL,
                            NOUN_CLASSES_IN_SWAHILI__BANTU23_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL1_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL2_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL3_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL4_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL5_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL6_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL7_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL8_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL9_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL10_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL11_UVAL,
                            NOUN_CLASSES_IN_WOLOF__WOL12_UVAL}},
    {NOUNTYPE_UFEAT.code, {NOUNTYPE__CLF_UVAL}},
    {NUMFORM_UFEAT.code,
     {NUMFORM__WORD_UVAL, NUMFORM__DIGIT_UVAL, NUMFORM__ROMAN_UVAL}},
    {NUMTYPE_UFEAT.code,
     {NUMTYPE__CARD_UVAL, NUMTYPE__ORD_UVAL, NUMTYPE__MULT_UVAL,
      NUMTYPE__FRAC_UVAL, NUMTYPE__SETS_UVAL, NUMTYPE__DIST_UVAL,
      NUMTYPE__RANGE_UVAL}},
    {NUMVALUE_UFEAT.code,
     {NUMVALUE__1_UVAL, NUMVALUE__2_UVAL, NUMVALUE__3_UVAL}},
    {NUMBER_UFEAT.code,
     {NUMBER__SING_UVAL, NUMBER__PLUR_UVAL, NUMBER__DUAL_UVAL, NUMBER__TRI_UVAL,
      NUMBER__PAUC_UVAL, NUMBER__GRPA_UVAL, NUMBER__GRPL_UVAL, NUMBER__INV_UVAL,
      NUMBER__COUNT_UVAL, NUMBER__PTAN_UVAL, NUMBER__COLL_UVAL}},
    {NUMBER__ABS_UFEAT.code, {NUMBER__ABS__SING_UVAL, NUMBER__ABS__PLUR_UVAL}},
    {NUMBER__DAT_UFEAT.code, {NUMBER__DAT__SING_UVAL, NUMBER__DAT__PLUR_UVAL}},
    {NUMBER__ERG_UFEAT.code, {NUMBER__ERG__SING_UVAL, NUMBER__ERG__PLUR_UVAL}},
    {NUMBER__OBJ_UFEAT.code, {NUMBER__OBJ__SING_UVAL, NUMBER__OBJ__PLUR_UVAL}},
    {NUMBER__PSED_UFEAT.code,
     {NUMBER__PSED__SING_UVAL, NUMBER__PSED__PLUR_UVAL}},
    {NUMBER__PSOR_UFEAT.code,
     {NUMBER__PSOR__SING_UVAL, NUMBER__PSOR__DUAL_UVAL,
      NUMBER__PSOR__PLUR_UVAL}},
    {NUMBER__SUBJ_UFEAT.code,
     {NUMBER__SUBJ__SING_UVAL, NUMBER__SUBJ__PLUR_UVAL}},
    {PARTTYPE_UFEAT.code,
     {PARTTYPE__MOD_UVAL, PARTTYPE__EMP_UVAL, PARTTYPE__INF_UVAL,
      PARTTYPE__INT_UVAL, PARTTYPE__NEG_UVAL, PARTTYPE__VBP_UVAL}},
    {PERSON_UFEAT.code,
     {PERSON__0_UVAL, PERSON__1_UVAL, PERSON__2_UVAL, PERSON__3_UVAL,
      PERSON__4_UVAL}},
    {PERSON__ABS_UFEAT.code,
     {PERSON__ABS__1_UVAL, PERSON__ABS__2_UVAL, PERSON__ABS__3_UVAL}},
    {PERSON__DAT_UFEAT.code,
     {PERSON__DAT__1_UVAL, PERSON__DAT__2_UVAL, PERSON__DAT__3_UVAL}},
    {PERSON__ERG_UFEAT.code,
     {PERSON__ERG__1_UVAL, PERSON__ERG__2_UVAL, PERSON__ERG__3_UVAL}},
    {PERSON__OBJ_UFEAT.code,
     {PERSON__OBJ__1_UVAL, PERSON__OBJ__2_UVAL, PERSON__OBJ__3_UVAL}},
    {PERSON__PSOR_UFEAT.code,
     {PERSON__PSOR__1_UVAL, PERSON__PSOR__2_UVAL, PERSON__PSOR__3_UVAL}},
    {PERSON__SUBJ_UFEAT.code,
     {PERSON__SUBJ__1_UVAL, PERSON__SUBJ__2_UVAL, PERSON__SUBJ__3_UVAL}},
    {POLARITY_UFEAT.code, {POLARITY__UPOS_UVAL, POLARITY__NEG_UVAL}},
    {POLITE_UFEAT.code,
     {POLITE__INFM_UVAL, POLITE__FORM_UVAL, POLITE__ELEV_UVAL,
      POLITE__HUMB_UVAL}},
    {POLITE__ABS_UFEAT.code, {POLITE__ABS__INFM_UVAL, POLITE__ABS__FORM_UVAL}},
    {POLITE__DAT_UFEAT.code, {POLITE__DAT__INFM_UVAL, POLITE__DAT__FORM_UVAL}},
    {POLITE__ERG_UFEAT.code, {POLITE__ERG__INFM_UVAL, POLITE__ERG__FORM_UVAL}},
    {POSS_UFEAT.code, {POSS__YES_UVAL}},
    {PREPCASE_UFEAT.code, {PREPCASE__NPR_UVAL, PREPCASE__PRE_UVAL}},
    {PRONTYPE_UFEAT.code,
     {PRONTYPE__PRS_UVAL, PRONTYPE__RCP_UVAL, PRONTYPE__ART_UVAL,
      PRONTYPE__INT_UVAL, PRONTYPE__REL_UVAL, PRONTYPE__EXC_UVAL,
      PRONTYPE__DEM_UVAL, PRONTYPE__EMP_UVAL, PRONTYPE__TOT_UVAL,
      PRONTYPE__NEG_UVAL, PRONTYPE__IND_UVAL}},
    {PUNCTSIDE_UFEAT.code, {PUNCTSIDE__INI_UVAL, PUNCTSIDE__FIN_UVAL}},
    {PUNCTTYPE_UFEAT.code,
     {PUNCTTYPE__PERI_UVAL, PUNCTTYPE__QEST_UVAL, PUNCTTYPE__EXCL_UVAL,
      PUNCTTYPE__QUOT_UVAL, PUNCTTYPE__BRCK_UVAL, PUNCTTYPE__COMM_UVAL,
      PUNCTTYPE__COLO_UVAL, PUNCTTYPE__SEMI_UVAL, PUNCTTYPE__DASH_UVAL}},
    {REFLEX_UFEAT.code, {REFLEX__YES_UVAL}},
    {STYLE_UFEAT.code,
     {STYLE__ARCH_UVAL, STYLE__RARE_UVAL, STYLE__FORM_UVAL, STYLE__COLL_UVAL,
      STYLE__VRNC_UVAL, STYLE__SLNG_UVAL, STYLE__EXPR_UVAL, STYLE__VULG_UVAL}},
    {SUBCAT_UFEAT.code, {SUBCAT__INTR_UVAL, SUBCAT__TRAN_UVAL}},
    {TENSE_UFEAT.code,
     {TENSE__PAST_UVAL, TENSE__PRES_UVAL, TENSE__FUT_UVAL, TENSE__IMP_UVAL,
      TENSE__PQP_UVAL}},
    {TYPO_UFEAT.code, {TYPO__YES_UVAL}},
    {VERBFORM_UFEAT.code,
     {VERBFORM__FIN_UVAL, VERBFORM__INF_UVAL, VERBFORM__SUP_UVAL,
      VERBFORM__PART_UVAL, VERBFORM__CONV_UVAL, VERBFORM__GDV_UVAL,
      VERBFORM__GER_UVAL, VERBFORM__VNOUN_UVAL}},
    {VERBTYPE_UFEAT.code,
     {VERBTYPE__AUX_UVAL, VERBTYPE__COP_UVAL, VERBTYPE__MOD_UVAL,
      VERBTYPE__LIGHT_UVAL}},
    {VOICE_UFEAT.code,
     {VOICE__ACT_UVAL, VOICE__MID_UVAL, VOICE__RCP_UVAL, VOICE__PASS_UVAL,
      VOICE__ANTIP_UVAL, VOICE__LFOC_UVAL, VOICE__BFOC_UVAL, VOICE__DIR_UVAL,
      VOICE__INV_UVAL, VOICE__CAU_UVAL}},
};

}  // namespace constants

#endif  // UDON2_CPP_SRC_CONSTANTS_H_
